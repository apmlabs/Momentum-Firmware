/*
 * Neighborhood v3 — 433.92 MHz awareness & replay tool
 * 5 views: Dashboard, Scan, Detail, Info, Library
 */
#include "neighborhood_remote.h"
#define TAG "Neighborhood"

// ============== Protocol detection + decode ==============

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

static void nr_decode_info(NRSignal* sig) {
    switch(sig->proto) {
    case NRProtoHoneywell:
        snprintf(sig->info, sizeof(sig->info), "Alarm TE=%d %db", sig->te, sig->bit_count);
        break;
    case NRProtoPT2262:
        if(sig->raw_len >= 3) {
            uint32_t v = ((uint32_t)sig->raw_frame[sig->raw_len-3] << 16) |
                         ((uint32_t)sig->raw_frame[sig->raw_len-2] << 8) |
                          sig->raw_frame[sig->raw_len-1];
            snprintf(sig->info, sizeof(sig->info), "Addr:%02X Cmd:%02X",
                (unsigned)((v >> 8) & 0xFF), (unsigned)(v & 0xFF));
        } else snprintf(sig->info, sizeof(sig->info), "PT2262 %db", sig->bit_count);
        break;
    case NRProtoEV1527:
        if(sig->raw_len >= 3) {
            uint32_t v = ((uint32_t)sig->raw_frame[0] << 16) |
                         ((uint32_t)sig->raw_frame[1] << 8) | sig->raw_frame[2];
            snprintf(sig->info, sizeof(sig->info), "A:%05lX D:%u",
                (unsigned long)((v >> 4) & 0xFFFFF), (unsigned)(v & 0x0F));
        } else snprintf(sig->info, sizeof(sig->info), "EV1527 %db", sig->bit_count);
        break;
    case NRProtoKeeloq:
        if(sig->raw_len >= 8) {
            uint32_t sn = (((uint32_t)sig->raw_frame[4] & 0x0F) << 24) |
                          ((uint32_t)sig->raw_frame[5] << 16) |
                          ((uint32_t)sig->raw_frame[6] << 8) | sig->raw_frame[7];
            sn >>= 4;
            uint8_t btn = sig->raw_frame[7] & 0x0F;
            snprintf(sig->info, sizeof(sig->info), "Sn:%07lX B:%u",
                (unsigned long)sn, btn);
        } else snprintf(sig->info, sizeof(sig->info), "Keeloq %db", sig->bit_count);
        break;
    case NRProtoFSK:
        snprintf(sig->info, sizeof(sig->info), "FSK TE=%d (FM476)", sig->te);
        break;
    default:
        snprintf(sig->info, sizeof(sig->info), "TE=%d %db", sig->te, sig->bit_count);
        break;
    }
}

// ============== Signal matching ==============

static int8_t nr_find_match(NRApp* app, NRProto proto, uint8_t* data, uint8_t len) {
    for(uint8_t i = 0; i < app->signal_count; i++) {
        if(app->signals[i].proto != proto) continue;
        if(proto == NRProtoPT2262 || proto == NRProtoEV1527) {
            if(app->signals[i].raw_len == len &&
               memcmp(app->signals[i].raw_frame, data, len) == 0) return i;
        } else if(proto == NRProtoHoneywell || proto == NRProtoKeeloq) {
            if(len >= 4 && app->signals[i].raw_len >= 4 &&
               memcmp(app->signals[i].raw_frame + app->signals[i].raw_len - 4,
                      data + len - 4, 4) == 0) return i;
        } else {
            uint8_t cmp = len < 4 ? len : 4;
            if(app->signals[i].raw_len >= cmp &&
               memcmp(app->signals[i].raw_frame, data, cmp) == 0) return i;
        }
    }
    return -1;
}

static uint8_t nr_find_slot(NRApp* app) {
    if(app->signal_count < NR_MAX_SIGNALS) return app->signal_count;
    uint8_t min_i = 0;
    for(uint8_t i = 1; i < NR_MAX_SIGNALS; i++)
        if(app->signals[i].hits < app->signals[min_i].hits) min_i = i;
    return min_i;
}

// ============== Autosave ==============

static void nr_autosave(NRApp* app, NRSignal* sig) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    storage_simply_mkdir(st, NR_AUTOSAVE_DIR);
    char path[80];
    snprintf(path, sizeof(path), "%s/%s_%04d.txt",
        NR_AUTOSAVE_DIR, nr_proto_name[sig->proto], app->autosave_seq++);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, path)) {
        flipper_format_write_header_cstr(ff, "Neighborhood Signal", 1);
        flipper_format_write_string_cstr(ff, "Proto", nr_proto_name[sig->proto]);
        uint32_t vals[3] = {sig->te, sig->bit_count, sig->hits};
        flipper_format_write_uint32(ff, "Info", vals, 3);
        flipper_format_write_string_cstr(ff, "Decode", sig->info);
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
        flipper_format_write_header_cstr(ff, "Neighborhood DB", 1);
        uint32_t cnt = app->signal_count;
        flipper_format_write_uint32(ff, "Count", &cnt, 1);
        for(uint8_t i = 0; i < app->signal_count; i++) {
            NRSignal* s = &app->signals[i];
            uint32_t hdr[4] = {s->proto, s->te, s->bit_count, s->hits};
            flipper_format_write_uint32(ff, "Sig", hdr, 4);
            flipper_format_write_string_cstr(ff, "Info", s->info);
            flipper_format_write_string_cstr(ff, "Name", s->name);
            if(s->raw_len > 0)
                flipper_format_write_hex(ff, "Data", s->raw_frame, s->raw_len);
        }
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

static void nr_load(NRApp* app) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    app->signal_count = 0;
    if(flipper_format_file_open_existing(ff, NR_SAVE_FILE)) {
        uint32_t ver = 0;
        FuriString* type = furi_string_alloc();
        if(flipper_format_read_header(ff, type, &ver)) {
            uint32_t cnt = 0;
            flipper_format_read_uint32(ff, "Count", &cnt, 1);
            if(cnt > NR_MAX_SIGNALS) cnt = NR_MAX_SIGNALS;
            FuriString* str = furi_string_alloc();
            for(uint32_t i = 0; i < cnt; i++) {
                NRSignal* s = &app->signals[i];
                memset(s, 0, sizeof(NRSignal));
                uint32_t hdr[4] = {0};
                if(!flipper_format_read_uint32(ff, "Sig", hdr, 4)) break;
                s->proto = hdr[0] < NRProtoCount ? hdr[0] : NRProtoBinRAW;
                s->te = hdr[1]; s->bit_count = hdr[2]; s->hits = hdr[3];
                if(flipper_format_read_string(ff, "Info", str))
                    snprintf(s->info, sizeof(s->info), "%s", furi_string_get_cstr(str));
                if(flipper_format_read_string(ff, "Name", str))
                    snprintf(s->name, sizeof(s->name), "%s", furi_string_get_cstr(str));
                uint8_t data[32];
                uint32_t data_len = 32;
                if(flipper_format_read_hex(ff, "Data", data, data_len)) {
                    memcpy(s->raw_frame, data, data_len);
                    s->raw_len = data_len;
                }
                app->signal_count++;
            }
            furi_string_free(str);
        }
        furi_string_free(type);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

// ============== RX callback (ISR context) ==============

static void nr_rx_cb(void* ctx, bool level, uint32_t duration) {
    NRApp* app = ctx;
    if(level) { app->rx_pulse = duration; return; }

    uint32_t h = app->rx_pulse, l = duration;

    // Sync gap (>5ms) = frame boundary
    if(l > 5000) {
        if(app->rx_bit_count >= 24 && app->rx_te_n > 0 && !app->rx_frame_ready) {
            uint16_t te = app->rx_te_sum / app->rx_te_n;
            uint8_t byte_len = (app->rx_bit_count + 7) / 8;
            if(byte_len > 32) byte_len = 32;
            memset(app->rx_frame_data, 0, 32);
            for(uint16_t i = 0; i < app->rx_bit_count && i < 256; i++) {
                if(app->rx_bits[i])
                    app->rx_frame_data[i / 8] |= (1 << (7 - (i % 8)));
            }
            app->rx_frame_te = te;
            app->rx_frame_bits = app->rx_bit_count;
            app->rx_frame_len = byte_len;
            app->rx_frame_ready = true;
        }
        app->rx_bit_count = 0;
        app->rx_te_sum = 0;
        app->rx_te_n = 0;
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

// ============== Radio control ==============

static void nr_start_rx(NRApp* app) {
    if(app->rx_active) return;
    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio_device, 433920000);
    app->rx_bit_count = 0;
    app->rx_te_sum = 0;
    app->rx_te_n = 0;
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

// ============== TX (replay) ==============

static void nr_transmit(NRApp* app, NRSignal* sig) {
    if(!nr_proto_replayable[sig->proto] || sig->raw_len == 0) return;
    bool was_rx = app->rx_active;
    if(was_rx) nr_stop_rx(app);

    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio_device, 433920000);

    uint16_t te = sig->te, te3 = te * 3;
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

static void nr_sort_signals(NRApp* app) {
    for(uint8_t i = 1; i < app->signal_count; i++) {
        NRSignal tmp = app->signals[i];
        int8_t j = i - 1;
        while(j >= 0) {
            bool swap = (app->sort == NRSortHits)
                ? app->signals[j].hits < tmp.hits
                : app->signals[j].last_seen < tmp.last_seen;
            if(!swap) break;
            app->signals[j + 1] = app->signals[j];
            j--;
        }
        app->signals[j + 1] = tmp;
    }
}

// Library: count replayable signals
static uint8_t nr_lib_count(NRApp* app) {
    uint8_t c = 0;
    for(uint8_t i = 0; i < app->signal_count; i++)
        if(nr_proto_replayable[app->signals[i].proto]) c++;
    return c;
}

// Library: get nth replayable signal index
static int8_t nr_lib_index(NRApp* app, uint8_t n) {
    uint8_t c = 0;
    for(uint8_t i = 0; i < app->signal_count; i++) {
        if(nr_proto_replayable[app->signals[i].proto]) {
            if(c == n) return i;
            c++;
        }
    }
    return -1;
}

// ============== Drawing ==============

static void nr_draw(Canvas* canvas, void* ctx) {
    NRApp* app = ctx;
    canvas_clear(canvas);
    char buf[64];

    if(app->view == NRViewDash) {
        // ── DASHBOARD ──
        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "NEIGHBORHOOD");
        canvas_draw_str(canvas, 0, 11, buf);
        canvas_set_font(canvas, FontSecondary);
        snprintf(buf, sizeof(buf), "%d devices", app->signal_count);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);

        canvas_draw_line(canvas, 0, 13, 127, 13);

        if(app->signal_count == 0) {
            canvas_draw_str(canvas, 10, 30, "No signals captured yet");
            canvas_draw_str(canvas, 10, 42, "Press OK to start scanning");
        } else {
            // Bar chart per protocol
            uint8_t counts[NRProtoCount] = {0};
            uint8_t max_count = 1;
            for(uint8_t i = 0; i < app->signal_count; i++) {
                counts[app->signals[i].proto]++;
                if(counts[app->signals[i].proto] > max_count)
                    max_count = counts[app->signals[i].proto];
            }

            uint8_t y = 16;
            for(uint8_t p = 0; p < NRProtoCount; p++) {
                if(counts[p] == 0) continue;
                // Icon + name
                snprintf(buf, sizeof(buf), "%s %-9s %d",
                    nr_proto_icon[p], nr_proto_name[p], counts[p]);
                canvas_draw_str(canvas, 0, y + 7, buf);
                // Bar
                uint8_t bar_w = (counts[p] * 40) / max_count;
                if(bar_w < 2) bar_w = 2;
                canvas_draw_rbox(canvas, 85, y, bar_w, 7, 1);
                y += 10;
                if(y > 50) break;
            }
        }

        // Footer
        canvas_draw_line(canvas, 0, 54, 127, 54);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 0, 63, "OK:Scan");
        uint8_t lc = nr_lib_count(app);
        if(lc > 0) {
            snprintf(buf, sizeof(buf), ">:Library(%d)", lc);
            canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, buf);
        }

    } else if(app->view == NRViewScan) {
        // ── SCAN ──
        canvas_set_font(canvas, FontPrimary);

        // Animated scan indicator
        const char* scan_frames[] = {"~   ", " ~  ", "  ~ ", "   ~", "  ~ ", " ~  "};
        const char* anim = scan_frames[app->scan_anim % 6];

        if(app->filter < 0) {
            snprintf(buf, sizeof(buf), "SCAN %s %s",
                anim, app->sort == NRSortHits ? "HITS" : "NEW");
        } else {
            snprintf(buf, sizeof(buf), "SCAN %s [%s]",
                anim, nr_proto_name[app->filter]);
        }
        canvas_draw_str(canvas, 0, 11, buf);
        canvas_set_font(canvas, FontSecondary);
        snprintf(buf, sizeof(buf), "%d", app->signal_count);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);

        canvas_draw_line(canvas, 0, 13, 127, 13);

        // Signal list (filtered)
        uint8_t vis = 0, row = 0;
        for(uint8_t i = 0; i < app->signal_count; i++) {
            NRSignal* s = &app->signals[i];
            if(app->filter >= 0 && s->proto != (NRProto)app->filter) continue;
            if(vis < app->sel) { vis++; continue; } // scroll offset
            if(row >= 4) break;

            uint8_t y = 16 + row * 10;
            bool selected = (vis == app->sel);
            if(selected) {
                canvas_draw_box(canvas, 0, y - 1, 128, 10);
                canvas_set_color(canvas, ColorWhite);
            }

            // Age indicator
            uint32_t age = app->tick - s->last_seen;
            char age_ch = age < 100 ? '*' : age < 400 ? '.' : ' ';

            snprintf(buf, sizeof(buf), "%c%3lu%s %-9s %.16s",
                age_ch, (unsigned long)s->hits, nr_proto_icon[s->proto],
                nr_proto_name[s->proto], s->info);
            buf[42] = 0;
            canvas_draw_str(canvas, 0, y + 7, buf);
            canvas_set_color(canvas, ColorBlack);
            vis++;
            row++;
        }

        // Footer
        canvas_draw_line(canvas, 0, 54, 127, 54);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 0, 63, "OK:Detail L/R:Sort");
        canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, "Hold:Filter");

    } else if(app->view == NRViewDetail) {
        // ── DETAIL ──
        if(app->sel >= app->signal_count) { app->view = NRViewScan; return; }
        NRSignal* s = &app->signals[app->sel];

        // Header with icon
        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "%s %s", nr_proto_icon[s->proto], nr_proto_name[s->proto]);
        canvas_draw_str(canvas, 0, 11, buf);
        snprintf(buf, sizeof(buf), "%lux", (unsigned long)s->hits);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);

        canvas_draw_line(canvas, 0, 13, 127, 13);
        canvas_set_font(canvas, FontSecondary);

        // Decoded info
        if(s->name[0]) {
            snprintf(buf, sizeof(buf), "\"%s\"", s->name);
            canvas_draw_str(canvas, 0, 22, buf);
            canvas_draw_str(canvas, 0, 31, s->info);
        } else {
            canvas_draw_str(canvas, 0, 22, s->info);
        }

        // TE + bits
        uint8_t info_y = s->name[0] ? 40 : 31;
        snprintf(buf, sizeof(buf), "TE:%d  Bits:%d", s->te, s->bit_count);
        canvas_draw_str(canvas, 0, info_y, buf);

        // Raw hex (first 8 bytes)
        char hex[48] = {0};
        for(uint8_t h = 0; h < s->raw_len && h < 8; h++)
            snprintf(hex + h * 3, 4, "%02X ", s->raw_frame[h]);
        canvas_draw_str(canvas, 0, info_y + 9, hex);

        // Mini waveform (bottom area)
        uint8_t wave_y = 48;
        canvas_draw_line(canvas, 0, wave_y, 127, wave_y);
        for(uint8_t x = 0; x < 128 && x < s->bit_count; x++) {
            uint8_t bit = (s->raw_frame[x / 8] >> (7 - (x % 8))) & 1;
            if(bit) {
                canvas_draw_line(canvas, x, wave_y + 1, x, wave_y + 4);
            } else {
                canvas_draw_dot(canvas, x, wave_y + 5);
            }
        }

        // Footer
        canvas_draw_line(canvas, 0, 56, 127, 56);
        canvas_set_font(canvas, FontSecondary);
        if(nr_proto_replayable[s->proto]) {
            canvas_draw_str(canvas, 0, 63, "OK:REPLAY");
        } else if(s->proto == NRProtoKeeloq) {
            canvas_draw_str(canvas, 0, 63, "Rolling code");
        } else if(s->proto == NRProtoHoneywell) {
            canvas_draw_str(canvas, 0, 63, "Alarm sensor");
        } else {
            canvas_draw_str(canvas, 0, 63, "Listen only");
        }
        canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, ">:Info Bk");

    } else if(app->view == NRViewInfo) {
        // ── PROTOCOL ENCYCLOPEDIA ──
        uint8_t proto_idx = 0;
        if(app->sel < app->signal_count)
            proto_idx = app->signals[app->sel].proto;

        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "%s %s",
            nr_proto_icon[proto_idx], nr_proto_name[proto_idx]);
        canvas_draw_str(canvas, 0, 11, buf);

        // Count devices of this type
        uint8_t dev_count = 0;
        for(uint8_t i = 0; i < app->signal_count; i++)
            if(app->signals[i].proto == (NRProto)proto_idx) dev_count++;
        snprintf(buf, sizeof(buf), "%d devs", dev_count);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);

        canvas_draw_line(canvas, 0, 13, 127, 13);
        canvas_set_font(canvas, FontSecondary);

        // Scrollable description text
        const char* desc = nr_proto_desc[proto_idx];
        uint8_t y = 22;
        uint8_t line = 0;
        const char* p = desc;
        while(*p && y < 56) {
            const char* nl = p;
            while(*nl && *nl != '\n') nl++;
            if(line >= app->info_scroll) {
                uint8_t len = nl - p;
                if(len >= sizeof(buf)) len = sizeof(buf) - 1;
                memcpy(buf, p, len);
                buf[len] = 0;
                canvas_draw_str(canvas, 0, y, buf);
                y += 9;
            }
            line++;
            p = *nl ? nl + 1 : nl;
        }

        canvas_draw_line(canvas, 0, 56, 127, 56);
        canvas_draw_str(canvas, 0, 63, "U/D:Scroll  Bk:back");

    } else if(app->view == NRViewLibrary) {
        // ── SIGNAL LIBRARY (replayable only) ──
        uint8_t lc = nr_lib_count(app);

        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "LIBRARY");
        canvas_draw_str(canvas, 0, 11, buf);
        snprintf(buf, sizeof(buf), "%d signals", lc);
        canvas_draw_str_aligned(canvas, 127, 11, AlignRight, AlignBottom, buf);

        canvas_draw_line(canvas, 0, 13, 127, 13);
        canvas_set_font(canvas, FontSecondary);

        if(lc == 0) {
            canvas_draw_str(canvas, 4, 28, "No replayable signals");
            canvas_draw_str(canvas, 4, 40, "Scan to find PT2262/EV1527");
        } else {
            uint8_t start = app->lib_sel > 3 ? app->lib_sel - 3 : 0;
            for(uint8_t n = start; n < lc && (n - start) < 4; n++) {
                int8_t idx = nr_lib_index(app, n);
                if(idx < 0) continue;
                NRSignal* s = &app->signals[idx];
                uint8_t y = 16 + (n - start) * 10;

                if(n == app->lib_sel) {
                    canvas_draw_box(canvas, 0, y - 1, 128, 10);
                    canvas_set_color(canvas, ColorWhite);
                }

                if(s->name[0]) {
                    snprintf(buf, sizeof(buf), "> %-9s \"%s\"",
                        nr_proto_name[s->proto], s->name);
                } else {
                    snprintf(buf, sizeof(buf), "> %-9s %s",
                        nr_proto_name[s->proto], s->info);
                }
                buf[42] = 0;
                canvas_draw_str(canvas, 0, y + 7, buf);
                canvas_set_color(canvas, ColorBlack);
            }
        }

        canvas_draw_line(canvas, 0, 54, 127, 54);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 0, 63, "OK:SEND  Bk:back");
    }
}

static void nr_input(InputEvent* event, void* ctx) {
    NRApp* app = ctx;
    furi_message_queue_put(app->event_queue, event, FuriWaitForever);
}

// ============== Process received frame (double-buffered) ==============

static void nr_process_frame(NRApp* app) {
    if(!app->rx_frame_ready) return;

    // Copy from ISR buffer to processing buffer
    app->proc_te = app->rx_frame_te;
    app->proc_bits = app->rx_frame_bits;
    app->proc_len = app->rx_frame_len;
    memcpy(app->proc_data, app->rx_frame_data, app->rx_frame_len);
    app->rx_frame_ready = false; // Release ISR buffer

    NRProto proto = nr_classify(app->proc_te, app->proc_bits, app->proc_data, app->proc_len);

    int8_t idx = nr_find_match(app, proto, app->proc_data, app->proc_len);
    if(idx >= 0) {
        // Cooldown: don't count hits faster than NR_HIT_COOLDOWN ticks
        if((app->tick - app->signals[idx].last_seen) >= NR_HIT_COOLDOWN) {
            app->signals[idx].hits++;
            app->signals[idx].last_seen = app->tick;
        }
        return;
    }

    // New signal
    uint8_t slot = nr_find_slot(app);
    NRSignal* s = &app->signals[slot];
    memset(s, 0, sizeof(NRSignal));
    s->proto = proto;
    s->te = app->proc_te;
    s->bit_count = app->proc_bits;
    s->hits = 1;
    s->last_seen = app->tick;
    s->raw_len = app->proc_len;
    memcpy(s->raw_frame, app->proc_data, app->proc_len);
    nr_decode_info(s);

    if(slot >= app->signal_count && app->signal_count < NR_MAX_SIGNALS)
        app->signal_count++;

    nr_autosave(app, s);
    notification_message(app->notifications, &sequence_blink_cyan_10);
}

// ============== Filtered selection helpers ==============

static uint8_t nr_filtered_count(NRApp* app) {
    if(app->filter < 0) return app->signal_count;
    uint8_t c = 0;
    for(uint8_t i = 0; i < app->signal_count; i++)
        if(app->signals[i].proto == (NRProto)app->filter) c++;
    return c;
}

// Get real index from filtered position
static int8_t nr_filtered_index(NRApp* app, uint8_t pos) {
    if(app->filter < 0) return pos < app->signal_count ? pos : -1;
    uint8_t c = 0;
    for(uint8_t i = 0; i < app->signal_count; i++) {
        if(app->signals[i].proto == (NRProto)app->filter) {
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
                } else if(event.key == InputKeyRight && nr_lib_count(app) > 0) {
                    app->lib_sel = 0;
                    app->view = NRViewLibrary;
                }

            } else if(app->view == NRViewScan) {
                uint8_t fc = nr_filtered_count(app);
                if(event.key == InputKeyBack) {
                    nr_stop_rx(app);
                    app->view = NRViewDash;
                } else if(event.key == InputKeyOk && fc > 0) {
                    // Map filtered sel to real index
                    int8_t ri = nr_filtered_index(app, app->sel);
                    if(ri >= 0) { app->sel = ri; app->view = NRViewDetail; }
                } else if(event.key == InputKeyUp && app->sel > 0) {
                    app->sel--;
                } else if(event.key == InputKeyDown && app->sel + 1 < fc) {
                    app->sel++;
                } else if(event.key == InputKeyLeft && event.type == InputTypeShort) {
                    app->sort = (app->sort == NRSortHits) ? NRSortRecent : NRSortHits;
                    nr_sort_signals(app);
                    app->sel = 0;
                } else if(event.key == InputKeyRight && event.type == InputTypeShort) {
                    app->sort = (app->sort == NRSortHits) ? NRSortRecent : NRSortHits;
                    nr_sort_signals(app);
                    app->sel = 0;
                } else if(event.type == InputTypeLong &&
                          (event.key == InputKeyLeft || event.key == InputKeyRight)) {
                    // Cycle protocol filter
                    if(event.key == InputKeyRight) {
                        app->filter++;
                        if(app->filter >= (int8_t)NRProtoCount) app->filter = -1;
                    } else {
                        app->filter--;
                        if(app->filter < -1) app->filter = NRProtoCount - 1;
                    }
                    app->sel = 0;
                }

            } else if(app->view == NRViewDetail) {
                if(event.key == InputKeyBack) {
                    app->view = NRViewScan;
                    app->sel = 0; // reset to top of filtered list
                } else if(event.key == InputKeyOk && app->sel < app->signal_count) {
                    NRSignal* s = &app->signals[app->sel];
                    if(nr_proto_replayable[s->proto]) {
                        notification_message(app->notifications, &sequence_blink_magenta_100);
                        nr_transmit(app, s);
                        notification_message(app->notifications, &sequence_blink_green_100);
                    }
                } else if(event.key == InputKeyRight) {
                    app->info_scroll = 0;
                    app->view = NRViewInfo;
                } else if(event.key == InputKeyUp && app->sel > 0) {
                    app->sel--;
                } else if(event.key == InputKeyDown && app->sel + 1 < app->signal_count) {
                    app->sel++;
                }

            } else if(app->view == NRViewInfo) {
                if(event.key == InputKeyBack) {
                    app->view = NRViewDetail;
                } else if(event.key == InputKeyUp && app->info_scroll > 0) {
                    app->info_scroll--;
                } else if(event.key == InputKeyDown) {
                    app->info_scroll++;
                }

            } else if(app->view == NRViewLibrary) {
                uint8_t lc = nr_lib_count(app);
                if(event.key == InputKeyBack) {
                    app->view = NRViewDash;
                } else if(event.key == InputKeyOk && lc > 0) {
                    int8_t ri = nr_lib_index(app, app->lib_sel);
                    if(ri >= 0) {
                        notification_message(app->notifications, &sequence_blink_magenta_100);
                        nr_transmit(app, &app->signals[ri]);
                        notification_message(app->notifications, &sequence_blink_green_100);
                    }
                } else if(event.key == InputKeyUp && app->lib_sel > 0) {
                    app->lib_sel--;
                } else if(event.key == InputKeyDown && app->lib_sel + 1 < lc) {
                    app->lib_sel++;
                }
            }
        }

tick:
        app->tick++;
        nr_process_frame(app);

        // Animate scan indicator
        if(app->view == NRViewScan && (app->tick % 8) == 0)
            app->scan_anim++;

        // Re-sort periodically
        if(app->view == NRViewScan && (app->tick % 60) == 0)
            nr_sort_signals(app);

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
