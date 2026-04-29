/*
 * Neighborhood v3 — 433.92 MHz device awareness & replay
 * Device-centric: groups signals by physical device (remote/sensor).
 * 3 views: Dashboard, Scan, Device detail (signals + protocol info).
 */
#include "neighborhood_remote.h"
#define TAG "Neighborhood"

// ============== Protocol classification ==============

static NRProto nr_classify(uint16_t te, uint16_t bits, uint8_t* data, uint8_t len) {
    if(len >= 3) {
        uint8_t ff = 0;
        for(uint8_t i = 0; i < len; i++) if(data[i] == 0xFF) ff++;
        if(ff > len / 3 && te < 70) return NRProtoFSK;
    }
    if(te >= 220 && te <= 360 && bits >= 50) return NRProtoKeeloq;
    if(te >= 125 && te <= 165 && bits >= 30) return NRProtoHoneywell;
    if(te >= 175 && te <= 215 && bits >= 16) return NRProtoPT2262;
    if(te >= 105 && te <= 130 && bits >= 20) return NRProtoEV1527;
    return NRProtoBinRAW;
}

// Extract device ID (for grouping) and signal ID (for dedup within device)
static uint32_t nr_device_id(NRProto proto, uint8_t* data, uint8_t len) {
    switch(proto) {
    case NRProtoPT2262:
        // Group by address byte (same remote)
        if(len >= 3) return data[len - 3];
        break;
    case NRProtoEV1527:
        // Group by 20-bit address
        if(len >= 3) return (((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2]) >> 4;
        break;
    case NRProtoKeeloq:
        // Group by serial (bytes 4-7, shifted)
        if(len >= 8) {
            uint32_t sn = (((uint32_t)data[4] & 0x0F) << 24) |
                          ((uint32_t)data[5] << 16) | ((uint32_t)data[6] << 8) | data[7];
            return sn >> 4;
        }
        break;
    case NRProtoHoneywell:
        // Group by last 4 bytes (serial area)
        if(len >= 4) return ((uint32_t)data[len-4] << 24) | ((uint32_t)data[len-3] << 16) |
                            ((uint32_t)data[len-2] << 8) | data[len-1];
        break;
    default:
        // Group by first 2 bytes + TE
        if(len >= 2) return ((uint32_t)data[0] << 8) | data[1];
        break;
    }
    return 0;
}

// Generate signal label (button/event name)
static void nr_signal_label(NRProto proto, uint8_t* data, uint8_t len, char* out, uint8_t out_sz) {
    switch(proto) {
    case NRProtoPT2262:
        if(len >= 3) {
            uint8_t cmd = data[len - 1];
            snprintf(out, out_sz, "Cmd:%02X", (unsigned)cmd);
        } else snprintf(out, out_sz, "Signal");
        break;
    case NRProtoEV1527:
        if(len >= 3) {
            uint8_t cmd = data[2] & 0x0F;
            snprintf(out, out_sz, "Btn:%u", (unsigned)cmd);
        } else snprintf(out, out_sz, "Signal");
        break;
    case NRProtoKeeloq:
        if(len >= 8) {
            uint8_t btn = data[7] & 0x0F;
            snprintf(out, out_sz, "Btn:S%u", (unsigned)btn);
        } else snprintf(out, out_sz, "Signal");
        break;
    case NRProtoHoneywell:
        snprintf(out, out_sz, "Event %db", len * 8);
        break;
    default:
        snprintf(out, out_sz, "%db", len * 8);
        break;
    }
}

// Generate device display name
static void nr_device_label(NRDevice* dev) {
    if(dev->name[0]) return; // user already named it
    switch(dev->proto) {
    case NRProtoPT2262:
        snprintf(dev->name, NR_MAX_NAME, "Remote %02X", (unsigned)(dev->device_id & 0xFF));
        break;
    case NRProtoEV1527:
        snprintf(dev->name, NR_MAX_NAME, "Remote %05lX", (unsigned long)(dev->device_id & 0xFFFFF));
        break;
    case NRProtoKeeloq:
        snprintf(dev->name, NR_MAX_NAME, "Fob %07lX", (unsigned long)(dev->device_id & 0xFFFFFFF));
        break;
    case NRProtoHoneywell:
        snprintf(dev->name, NR_MAX_NAME, "Alarm %04lX", (unsigned long)(dev->device_id & 0xFFFF));
        break;
    case NRProtoFSK:
        snprintf(dev->name, NR_MAX_NAME, "FSK TE=%u", dev->te);
        break;
    default:
        snprintf(dev->name, NR_MAX_NAME, "Dev TE=%u", dev->te);
        break;
    }
}

// ============== Device/signal matching ==============

static int8_t nr_find_device(NRApp* app, NRProto proto, uint32_t dev_id) {
    for(uint8_t i = 0; i < app->device_count; i++)
        if(app->devices[i].proto == proto && app->devices[i].device_id == dev_id)
            return i;
    return -1;
}

// Find signal within device (exact raw match)
static int8_t nr_find_signal(NRDevice* dev, uint8_t* data, uint8_t len) {
    for(uint8_t i = 0; i < dev->sig_count; i++) {
        if(dev->sigs[i].raw_len == len &&
           memcmp(dev->sigs[i].raw_frame, data, len) == 0) return i;
    }
    return -1;
}

// ============== Autosave ==============

static void nr_autosave(NRApp* app, NRDevice* dev, NRSignalEntry* sig) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    storage_simply_mkdir(st, NR_AUTOSAVE_DIR);
    char path[80];
    snprintf(path, sizeof(path), "%s/%s_%04d.txt",
        NR_AUTOSAVE_DIR, nr_proto_name[dev->proto], app->autosave_seq++);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, path)) {
        flipper_format_write_header_cstr(ff, "Neighborhood Signal", 1);
        flipper_format_write_string_cstr(ff, "Proto", nr_proto_name[dev->proto]);
        flipper_format_write_string_cstr(ff, "Device", dev->name);
        flipper_format_write_string_cstr(ff, "Signal", sig->label);
        uint32_t vals[2] = {dev->te, sig->bit_count};
        flipper_format_write_uint32(ff, "Info", vals, 2);
        if(sig->raw_len > 0)
            flipper_format_write_hex(ff, "Data", sig->raw_frame, sig->raw_len);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

// ============== Save/Load ==============

static void nr_save(NRApp* app) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, NR_SAVE_FILE)) {
        flipper_format_write_header_cstr(ff, "Neighborhood DB", 2);
        uint32_t cnt = app->device_count;
        flipper_format_write_uint32(ff, "Count", &cnt, 1);
        for(uint8_t i = 0; i < app->device_count; i++) {
            NRDevice* d = &app->devices[i];
            uint32_t hdr[5] = {d->proto, d->te, d->device_id, d->hits, d->sig_count};
            flipper_format_write_uint32(ff, "Dev", hdr, 5);
            flipper_format_write_string_cstr(ff, "Name", d->name);
            for(uint8_t s = 0; s < d->sig_count; s++) {
                flipper_format_write_string_cstr(ff, "SigLabel", d->sigs[s].label);
                uint32_t sb = d->sigs[s].bit_count;
                flipper_format_write_uint32(ff, "SigBits", &sb, 1);
                if(d->sigs[s].raw_len > 0)
                    flipper_format_write_hex(ff, "SigData", d->sigs[s].raw_frame, d->sigs[s].raw_len);
            }
        }
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

static void nr_load(NRApp* app) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    app->device_count = 0;
    if(flipper_format_file_open_existing(ff, NR_SAVE_FILE)) {
        uint32_t ver = 0;
        FuriString* type = furi_string_alloc();
        if(flipper_format_read_header(ff, type, &ver)) {
            uint32_t cnt = 0;
            flipper_format_read_uint32(ff, "Count", &cnt, 1);
            if(cnt > NR_MAX_DEVICES) cnt = NR_MAX_DEVICES;
            FuriString* str = furi_string_alloc();
            for(uint32_t i = 0; i < cnt; i++) {
                NRDevice* d = &app->devices[i];
                memset(d, 0, sizeof(NRDevice));
                uint32_t hdr[5] = {0};
                if(!flipper_format_read_uint32(ff, "Dev", hdr, 5)) break;
                d->proto = hdr[0] < NRProtoCount ? hdr[0] : NRProtoBinRAW;
                d->te = hdr[1]; d->device_id = hdr[2]; d->hits = hdr[3];
                uint8_t sc = hdr[4]; if(sc > NR_MAX_SIGNALS) sc = NR_MAX_SIGNALS;
                if(flipper_format_read_string(ff, "Name", str))
                    snprintf(d->name, NR_MAX_NAME, "%s", furi_string_get_cstr(str));
                for(uint8_t s = 0; s < sc; s++) {
                    if(flipper_format_read_string(ff, "SigLabel", str))
                        snprintf(d->sigs[s].label, 20, "%s", furi_string_get_cstr(str));
                    uint32_t sb = 0;
                    flipper_format_read_uint32(ff, "SigBits", &sb, 1);
                    d->sigs[s].bit_count = sb;
                    uint8_t data[32]; uint32_t dl = 32;
                    if(flipper_format_read_hex(ff, "SigData", data, dl)) {
                        memcpy(d->sigs[s].raw_frame, data, dl);
                        d->sigs[s].raw_len = dl;
                    }
                    d->sig_count++;
                }
                app->device_count++;
            }
            furi_string_free(str);
        }
        furi_string_free(type);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

// ============== RX callback (ISR) ==============

static void nr_rx_cb(void* ctx, bool level, uint32_t duration) {
    NRApp* app = ctx;
    if(level) { app->rx_pulse = duration; return; }
    uint32_t h = app->rx_pulse, l = duration;

    if(l > 5000) {
        if(app->rx_bit_count >= 24 && app->rx_te_n > 0 && !app->rx_frame_ready) {
            uint16_t te = app->rx_te_sum / app->rx_te_n;
            uint8_t byte_len = (app->rx_bit_count + 7) / 8;
            if(byte_len > 32) byte_len = 32;
            memset(app->rx_frame_data, 0, 32);
            for(uint16_t i = 0; i < app->rx_bit_count && i < 256; i++)
                if(app->rx_bits[i])
                    app->rx_frame_data[i / 8] |= (1 << (7 - (i % 8)));
            app->rx_frame_te = te;
            app->rx_frame_bits = app->rx_bit_count;
            app->rx_frame_len = byte_len;
            app->rx_frame_ready = true;
        }
        app->rx_bit_count = 0; app->rx_te_sum = 0; app->rx_te_n = 0;
        return;
    }
    if(h < 50 || l < 50) return;
    uint32_t shorter = h < l ? h : l;
    if(shorter < 500) { app->rx_te_sum += shorter; app->rx_te_n++; }
    if(app->rx_bit_count < 128) {
        uint32_t te_est = app->rx_te_n > 0 ? app->rx_te_sum / app->rx_te_n : 200;
        app->rx_bits[app->rx_bit_count++] = (h > te_est * 2) ? 1 : 0;
    }
}

// ============== Radio ==============

static void nr_start_rx(NRApp* app) {
    if(app->rx_active) return;
    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio_device, 433920000);
    app->rx_bit_count = 0; app->rx_te_sum = 0; app->rx_te_n = 0;
    app->rx_frame_ready = false;
    subghz_worker_set_pair_callback(app->worker, (SubGhzWorkerPairCallback)nr_rx_cb);
    subghz_worker_set_context(app->worker, app);
    subghz_devices_start_async_rx(app->radio_device, subghz_worker_rx_callback, app->worker);
    subghz_worker_start(app->worker);
    app->rx_active = true;
}

static void nr_stop_rx(NRApp* app) {
    if(!app->rx_active) return;
    subghz_worker_stop(app->worker);
    subghz_devices_stop_async_rx(app->radio_device);
    subghz_devices_idle(app->radio_device);
    app->rx_active = false;
}

// ============== TX ==============

static void nr_transmit(NRApp* app, NRDevice* dev, NRSignalEntry* sig) {
    if(!nr_proto_replayable[dev->proto] || sig->raw_len == 0) return;
    bool was_rx = app->rx_active;
    if(was_rx) nr_stop_rx(app);
    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio_device, 433920000);
    uint16_t te = dev->te, te3 = te * 3;
    for(int rep = 0; rep < 6; rep++) {
        subghz_devices_set_tx(app->radio_device);
        furi_hal_gpio_write(&gpio_cc1101_g0, true);
        furi_delay_us(te);
        furi_hal_gpio_write(&gpio_cc1101_g0, false);
        furi_delay_us(te * 31);
        for(uint16_t i = 0; i < sig->bit_count && i < 256; i++) {
            uint8_t bit = (sig->raw_frame[i / 8] >> (7 - (i % 8))) & 1;
            furi_hal_gpio_write(&gpio_cc1101_g0, true);
            furi_delay_us(bit ? te3 : te);
            furi_hal_gpio_write(&gpio_cc1101_g0, false);
            furi_delay_us(bit ? te : te3);
        }
        furi_hal_gpio_write(&gpio_cc1101_g0, false);
        subghz_devices_idle(app->radio_device);
        furi_delay_ms(8);
    }
    if(was_rx) nr_start_rx(app);
}

// ============== Sort ==============

static void nr_sort(NRApp* app) {
    for(uint8_t i = 1; i < app->device_count; i++) {
        NRDevice tmp = app->devices[i];
        int8_t j = i - 1;
        while(j >= 0) {
            bool swap = (app->sort == NRSortHits)
                ? app->devices[j].hits < tmp.hits
                : app->devices[j].last_seen < tmp.last_seen;
            if(!swap) break;
            app->devices[j + 1] = app->devices[j];
            j--;
        }
        app->devices[j + 1] = tmp;
    }
}

// ============== Process frame ==============

static void nr_process_frame(NRApp* app) {
    if(!app->rx_frame_ready) return;
    app->proc_te = app->rx_frame_te;
    app->proc_bits = app->rx_frame_bits;
    app->proc_len = app->rx_frame_len;
    memcpy(app->proc_data, app->rx_frame_data, app->rx_frame_len);
    app->rx_frame_ready = false;

    NRProto proto = nr_classify(app->proc_te, app->proc_bits, app->proc_data, app->proc_len);
    uint32_t did = nr_device_id(proto, app->proc_data, app->proc_len);

    int8_t di = nr_find_device(app, proto, did);
    if(di >= 0) {
        NRDevice* d = &app->devices[di];
        // Cooldown
        if((app->tick - d->last_seen) >= NR_HIT_COOLDOWN) {
            d->hits++;
            d->last_seen = app->tick;
        }
        // Check if this is a new signal (button) for existing device
        if(nr_find_signal(d, app->proc_data, app->proc_len) < 0 &&
           d->sig_count < NR_MAX_SIGNALS) {
            NRSignalEntry* s = &d->sigs[d->sig_count];
            memset(s, 0, sizeof(NRSignalEntry));
            s->raw_len = app->proc_len;
            s->bit_count = app->proc_bits;
            memcpy(s->raw_frame, app->proc_data, app->proc_len);
            nr_signal_label(proto, app->proc_data, app->proc_len, s->label, sizeof(s->label));
            d->sig_count++;
            nr_autosave(app, d, s);
            notification_message(app->notifications, &sequence_blink_green_10);
        }
        return;
    }

    // New device
    uint8_t slot = app->device_count;
    if(slot >= NR_MAX_DEVICES) {
        // Evict lowest-hit device
        slot = 0;
        for(uint8_t i = 1; i < NR_MAX_DEVICES; i++)
            if(app->devices[i].hits < app->devices[slot].hits) slot = i;
    }

    NRDevice* d = &app->devices[slot];
    memset(d, 0, sizeof(NRDevice));
    d->proto = proto;
    d->te = app->proc_te;
    d->device_id = did;
    d->hits = 1;
    d->last_seen = app->tick;

    // First signal
    NRSignalEntry* s = &d->sigs[0];
    s->raw_len = app->proc_len;
    s->bit_count = app->proc_bits;
    memcpy(s->raw_frame, app->proc_data, app->proc_len);
    nr_signal_label(proto, app->proc_data, app->proc_len, s->label, sizeof(s->label));
    d->sig_count = 1;

    nr_device_label(d);

    if(slot >= app->device_count && app->device_count < NR_MAX_DEVICES)
        app->device_count++;

    nr_autosave(app, d, s);
    notification_message(app->notifications, &sequence_blink_cyan_10);
}

// ============== Drawing ==============

static void nr_draw(Canvas* canvas, void* ctx) {
    NRApp* app = ctx;
    canvas_clear(canvas);
    char buf[64];

    if(app->view == NRViewDash) {
        // ── DASHBOARD ──
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 0, 11, "NEIGHBORHOOD");
        snprintf(buf, sizeof(buf), "%d devs", app->device_count);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);
        canvas_draw_line(canvas, 0, 13, 127, 13);
        canvas_set_font(canvas, FontSecondary);

        if(app->device_count == 0) {
            canvas_draw_str(canvas, 10, 30, "No devices found yet");
            canvas_draw_str(canvas, 10, 42, "Press OK to start scanning");
        } else {
            uint8_t counts[NRProtoCount] = {0};
            uint8_t max_c = 1;
            for(uint8_t i = 0; i < app->device_count; i++) {
                counts[app->devices[i].proto]++;
                if(counts[app->devices[i].proto] > max_c)
                    max_c = counts[app->devices[i].proto];
            }
            uint8_t y = 16;
            for(uint8_t p = 0; p < NRProtoCount; p++) {
                if(!counts[p]) continue;
                snprintf(buf, sizeof(buf), "%s %-9s %d", nr_proto_icon[p], nr_proto_name[p], counts[p]);
                canvas_draw_str(canvas, 0, y + 7, buf);
                uint8_t bw = (counts[p] * 40) / max_c;
                if(bw < 2) bw = 2;
                canvas_draw_rbox(canvas, 85, y, bw, 7, 1);
                y += 10;
                if(y > 50) break;
            }
        }
        canvas_draw_line(canvas, 0, 54, 127, 54);
        canvas_draw_str(canvas, 0, 63, "OK:Scan  >:Devices");

    } else if(app->view == NRViewScan) {
        // ── SCAN ──
        canvas_set_font(canvas, FontPrimary);
        const char* anim[] = {"~   ", " ~  ", "  ~ ", "   ~", "  ~ ", " ~  "};
        if(app->filter < 0)
            snprintf(buf, sizeof(buf), "SCAN %s %s",
                anim[app->scan_anim % 6], app->sort == NRSortHits ? "HITS" : "NEW");
        else
            snprintf(buf, sizeof(buf), "SCAN %s [%s]",
                anim[app->scan_anim % 6], nr_proto_name[app->filter]);
        canvas_draw_str(canvas, 0, 11, buf);
        snprintf(buf, sizeof(buf), "%d", app->device_count);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);
        canvas_draw_line(canvas, 0, 13, 127, 13);
        canvas_set_font(canvas, FontSecondary);

        // Device list (filtered)
        uint8_t vis = 0, row = 0;
        for(uint8_t i = 0; i < app->device_count && row < 4; i++) {
            NRDevice* d = &app->devices[i];
            if(app->filter >= 0 && d->proto != (NRProto)app->filter) continue;
            if(vis < app->sel) { vis++; continue; }

            uint8_t y = 16 + row * 10;
            if(vis == app->sel) {
                canvas_draw_box(canvas, 0, y - 1, 128, 10);
                canvas_set_color(canvas, ColorWhite);
            }
            uint32_t age = app->tick - d->last_seen;
            char age_c = age < 100 ? '*' : age < 400 ? '.' : ' ';
            snprintf(buf, sizeof(buf), "%c%3lu%s %-8s %s",
                age_c, (unsigned long)d->hits, nr_proto_icon[d->proto],
                d->name, d->sig_count > 1 ? "+" : "");
            buf[42] = 0;
            canvas_draw_str(canvas, 0, y + 7, buf);
            canvas_set_color(canvas, ColorBlack);
            vis++; row++;
        }

        canvas_draw_line(canvas, 0, 54, 127, 54);
        canvas_draw_str(canvas, 0, 63, "OK:Open L/R:Sort");
        canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, "Hold:Filter");

    } else if(app->view == NRViewDevice) {
        // ── DEVICE DETAIL ──
        if(app->dev_sel >= app->device_count) { app->view = NRViewScan; return; }
        NRDevice* d = &app->devices[app->dev_sel];

        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "%s %s", nr_proto_icon[d->proto], d->name);
        canvas_draw_str(canvas, 0, 11, buf);
        snprintf(buf, sizeof(buf), "%lux", (unsigned long)d->hits);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);
        canvas_draw_line(canvas, 0, 13, 127, 13);
        canvas_set_font(canvas, FontSecondary);

        // Content: signals first, then protocol info — all scrollable
        uint8_t y_start = 16;
        int8_t line = -(int8_t)app->dev_scroll;

        // -- Signals section --
        for(uint8_t s = 0; s < d->sig_count; s++) {
            if(line >= 0 && line < 4) {
                uint8_t y = y_start + line * 10;
                bool selected = (app->sig_sel == s);
                if(selected) {
                    canvas_draw_box(canvas, 0, y - 1, 128, 10);
                    canvas_set_color(canvas, ColorWhite);
                }
                bool repl = nr_proto_replayable[d->proto];
                char hex[16] = {0};
                for(uint8_t h = 0; h < d->sigs[s].raw_len && h < 4; h++)
                    snprintf(hex + h * 3, 4, "%02X ", d->sigs[s].raw_frame[h]);

                snprintf(buf, sizeof(buf), " %s %-10s %s",
                    repl ? ">" : " ", d->sigs[s].label, hex);
                buf[42] = 0;
                canvas_draw_str(canvas, 0, y + 7, buf);
                canvas_set_color(canvas, ColorBlack);
            }
            line++;
        }

        // -- Separator --
        if(line >= 0 && line < 4) {
            uint8_t y = y_start + line * 10 + 3;
            canvas_draw_line(canvas, 0, y, 127, y);
        }
        line++;

        // -- Protocol description (scrollable) --
        const char* desc = nr_proto_desc[d->proto];
        const char* p = desc;
        while(*p) {
            const char* nl = p;
            while(*nl && *nl != '\n') nl++;
            if(line >= 0 && line < 4) {
                uint8_t len = nl - p;
                if(len >= sizeof(buf)) len = sizeof(buf) - 1;
                memcpy(buf, p, len); buf[len] = 0;
                uint8_t y = y_start + line * 10;
                canvas_draw_str(canvas, 2, y + 7, buf);
            }
            line++;
            p = *nl ? nl + 1 : nl;
        }

        // Footer
        canvas_draw_line(canvas, 0, 56, 127, 56);
        if(nr_proto_replayable[d->proto] && app->sig_sel < d->sig_count) {
            canvas_draw_str(canvas, 0, 63, "OK:SEND  U/D:Scroll");
        } else {
            canvas_draw_str(canvas, 0, 63, "U/D:Scroll  Bk:back");
        }
    }
}

static void nr_input(InputEvent* event, void* ctx) {
    NRApp* app = ctx;
    furi_message_queue_put(app->event_queue, event, FuriWaitForever);
}

// ============== Filtered helpers ==============

static uint8_t nr_filtered_count(NRApp* app) {
    if(app->filter < 0) return app->device_count;
    uint8_t c = 0;
    for(uint8_t i = 0; i < app->device_count; i++)
        if(app->devices[i].proto == (NRProto)app->filter) c++;
    return c;
}

static int8_t nr_filtered_index(NRApp* app, uint8_t pos) {
    if(app->filter < 0) return pos < app->device_count ? pos : -1;
    uint8_t c = 0;
    for(uint8_t i = 0; i < app->device_count; i++) {
        if(app->devices[i].proto == (NRProto)app->filter) {
            if(c == pos) return i;
            c++;
        }
    }
    return -1;
}

// ============== Main ==============

int32_t neighborhood_remote_app(void* p) {
    UNUSED(p);
    NRApp* app = malloc(sizeof(NRApp));
    memset(app, 0, sizeof(NRApp));
    app->filter = -1;

    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, nr_draw, app);
    view_port_input_callback_set(app->view_port, nr_input, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    subghz_devices_init();
    app->radio_device = subghz_devices_get_by_name(SUBGHZ_DEVICE_CC1101_INT_NAME);
    subghz_devices_begin(app->radio_device);
    subghz_devices_reset(app->radio_device);
    app->worker = subghz_worker_alloc();

    nr_load(app);
    app->view = NRViewDash;

    bool running = true;
    InputEvent event;
    while(running) {
        if(furi_message_queue_get(app->event_queue, &event, 50) == FuriStatusOk) {
            if(event.type != InputTypeShort && event.type != InputTypeLong) goto tick;

            if(app->view == NRViewDash) {
                if(event.key == InputKeyBack) {
                    running = false;
                } else if(event.key == InputKeyOk) {
                    app->sel = 0;
                    app->view = NRViewScan;
                    nr_start_rx(app);
                } else if(event.key == InputKeyRight && app->device_count > 0) {
                    app->dev_sel = 0;
                    app->sig_sel = 0;
                    app->dev_scroll = 0;
                    app->view = NRViewDevice;
                }

            } else if(app->view == NRViewScan) {
                uint8_t fc = nr_filtered_count(app);
                if(event.key == InputKeyBack) {
                    nr_stop_rx(app);
                    app->view = NRViewDash;
                } else if(event.key == InputKeyOk && fc > 0) {
                    int8_t ri = nr_filtered_index(app, app->sel);
                    if(ri >= 0) {
                        app->dev_sel = ri;
                        app->sig_sel = 0;
                        app->dev_scroll = 0;
                        app->view = NRViewDevice;
                    }
                } else if(event.key == InputKeyUp && app->sel > 0) {
                    app->sel--;
                } else if(event.key == InputKeyDown && app->sel + 1 < fc) {
                    app->sel++;
                } else if(event.type == InputTypeShort &&
                          (event.key == InputKeyLeft || event.key == InputKeyRight)) {
                    app->sort = (app->sort == NRSortHits) ? NRSortRecent : NRSortHits;
                    nr_sort(app); app->sel = 0;
                } else if(event.type == InputTypeLong &&
                          (event.key == InputKeyLeft || event.key == InputKeyRight)) {
                    if(event.key == InputKeyRight) {
                        app->filter++;
                        if(app->filter >= (int8_t)NRProtoCount) app->filter = -1;
                    } else {
                        app->filter--;
                        if(app->filter < -1) app->filter = NRProtoCount - 1;
                    }
                    app->sel = 0;
                }

            } else if(app->view == NRViewDevice) {
                NRDevice* d = app->dev_sel < app->device_count ? &app->devices[app->dev_sel] : NULL;
                if(event.key == InputKeyBack) {
                    app->view = NRViewScan;
                    app->sel = 0;
                } else if(event.key == InputKeyOk && d && app->sig_sel < d->sig_count) {
                    if(nr_proto_replayable[d->proto]) {
                        notification_message(app->notifications, &sequence_blink_magenta_100);
                        nr_transmit(app, d, &d->sigs[app->sig_sel]);
                        notification_message(app->notifications, &sequence_blink_green_100);
                    }
                } else if(event.key == InputKeyUp) {
                    if(app->sig_sel > 0) {
                        app->sig_sel--;
                        if(app->sig_sel < app->dev_scroll) app->dev_scroll = app->sig_sel;
                    } else if(app->dev_scroll > 0) {
                        app->dev_scroll--;
                    }
                } else if(event.key == InputKeyDown) {
                    if(d && app->sig_sel + 1 < d->sig_count) {
                        app->sig_sel++;
                    }
                    app->dev_scroll++;
                } else if(event.key == InputKeyLeft && app->dev_sel > 0) {
                    app->dev_sel--;
                    app->sig_sel = 0; app->dev_scroll = 0;
                } else if(event.key == InputKeyRight && d &&
                          app->dev_sel + 1 < app->device_count) {
                    app->dev_sel++;
                    app->sig_sel = 0; app->dev_scroll = 0;
                }
            }
        }

tick:
        app->tick++;
        nr_process_frame(app);
        if(app->view == NRViewScan && (app->tick % 8) == 0) app->scan_anim++;
        if(app->view == NRViewScan && (app->tick % 60) == 0) nr_sort(app);
        view_port_update(app->view_port);
    }

    nr_save(app);
    nr_stop_rx(app);
    subghz_worker_free(app->worker);
    subghz_devices_end(app->radio_device);
    subghz_devices_deinit();
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_message_queue_free(app->event_queue);
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);
    free(app);
    return 0;
}
