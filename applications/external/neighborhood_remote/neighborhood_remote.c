/*
 * Neighborhood — 433.92 MHz awareness & replay tool
 * Auto-decodes Honeywell, PT2262, EV1527, Keeloq from raw pulses.
 * Single-frame decode (no repeat match needed).
 */
#include "neighborhood_remote.h"
#define TAG "Neighborhood"

// ============== Protocol detection + decode ==============

static NRProto nr_classify(uint16_t te, uint16_t bits, uint8_t* data, uint8_t len) {
    // Check for FSK-as-OOK (high FF content)
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
        // Can't Manchester-decode from BinRAW, show what we can
        snprintf(sig->info, sizeof(sig->info), "Alarm TE=%d %db", sig->te, sig->bit_count);
        break;
    case NRProtoPT2262:
        if(sig->raw_len >= 3) {
            uint32_t v = (sig->raw_frame[sig->raw_len-3] << 16) |
                         (sig->raw_frame[sig->raw_len-2] << 8) |
                          sig->raw_frame[sig->raw_len-1];
            snprintf(sig->info, sizeof(sig->info), "Addr:%02lX Cmd:%02lX",
                (unsigned long)((v >> 8) & 0xFF), (unsigned long)(v & 0xFF));
        } else {
            snprintf(sig->info, sizeof(sig->info), "PT2262 %db", sig->bit_count);
        }
        break;
    case NRProtoEV1527:
        if(sig->raw_len >= 3) {
            uint32_t v = (sig->raw_frame[0] << 16) | (sig->raw_frame[1] << 8) | sig->raw_frame[2];
            uint32_t addr = (v >> 4) & 0xFFFFF;
            uint8_t cmd = v & 0x0F;
            snprintf(sig->info, sizeof(sig->info), "A:%05lX D:%X", (unsigned long)addr, cmd);
        } else {
            snprintf(sig->info, sizeof(sig->info), "EV1527 %db", sig->bit_count);
        }
        break;
    case NRProtoKeeloq:
        if(sig->raw_len >= 8) {
            uint32_t sn = ((sig->raw_frame[4] & 0x0F) << 24) |
                          (sig->raw_frame[5] << 16) |
                          (sig->raw_frame[6] << 8) |
                           sig->raw_frame[7];
            sn >>= 4;
            uint8_t btn = sig->raw_frame[7] & 0x0F;
            snprintf(sig->info, sizeof(sig->info), "Sn:%07lX B:%X", (unsigned long)sn, btn);
        } else {
            snprintf(sig->info, sizeof(sig->info), "Keeloq %db", sig->bit_count);
        }
        break;
    case NRProtoFSK:
        snprintf(sig->info, sizeof(sig->info), "FSK TE=%d (use FM476)", sig->te);
        break;
    default:
        snprintf(sig->info, sizeof(sig->info), "TE=%d %db", sig->te, sig->bit_count);
        break;
    }
}

// ============== Signal matching (dedup by proto+data) ==============

static int8_t nr_find_match(NRApp* app, NRProto proto, uint8_t* data, uint8_t len) {
    for(uint8_t i = 0; i < app->signal_count; i++) {
        if(app->signals[i].proto != proto) continue;
        // For Honeywell/Keeloq: match on TE range (same device class)
        // For PT2262/EV1527: match on exact data (same button)
        if(proto == NRProtoPT2262 || proto == NRProtoEV1527) {
            if(app->signals[i].raw_len == len &&
               memcmp(app->signals[i].raw_frame, data, len) == 0) return i;
        } else if(proto == NRProtoHoneywell || proto == NRProtoKeeloq) {
            // Match on last 4 bytes (serial area)
            if(len >= 4 && app->signals[i].raw_len >= 4 &&
               memcmp(app->signals[i].raw_frame + app->signals[i].raw_len - 4,
                      data + len - 4, 4) == 0) return i;
        } else {
            // BinRAW/FSK: match on first 4 bytes
            uint8_t cmp = len < 4 ? len : 4;
            if(app->signals[i].raw_len >= cmp &&
               memcmp(app->signals[i].raw_frame, data, cmp) == 0) return i;
        }
    }
    return -1;
}

static uint8_t nr_find_slot(NRApp* app) {
    if(app->signal_count < NR_MAX_SIGNALS) return app->signal_count;
    // Evict lowest-hit signal
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

// ============== Save/Load device database ==============

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
                uint32_t hdr[4] = {0};
                if(!flipper_format_read_uint32(ff, "Sig", hdr, 4)) break;
                s->proto = hdr[0] < NRProtoCount ? hdr[0] : NRProtoBinRAW;
                s->te = hdr[1]; s->bit_count = hdr[2]; s->hits = hdr[3];
                if(flipper_format_read_string(ff, "Info", str))
                    snprintf(s->info, sizeof(s->info), "%s", furi_string_get_cstr(str));
                s->raw_len = (s->bit_count + 7) / 8;
                if(s->raw_len > 32) s->raw_len = 32;
                flipper_format_read_hex(ff, "Data", s->raw_frame, s->raw_len);
                app->signal_count++;
            }
            furi_string_free(str);
        }
        furi_string_free(type);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

// ============== Radio RX ==============

static void nr_rx_cb(void* ctx, bool level, uint32_t duration) {
    NRApp* app = ctx;
    if(app->rx_frame_ready) return; // previous frame not consumed yet

    if(level) {
        // HIGH pulse — save duration
        app->rx_pulse = duration;
        return;
    }

    // LOW gap — decode pulse+gap pair
    uint32_t h = app->rx_pulse;
    uint32_t l = duration;

    // Sync gap detection (>5ms)
    if(l > 5000) {
        if(app->rx_bit_count >= 12 && app->rx_te_n > 0) {
            // Frame complete — package it
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
        // Reset for next frame
        app->rx_bit_count = 0;
        app->rx_te_sum = 0;
        app->rx_te_n = 0;
        return;
    }

    // Skip very short pulses (noise)
    if(h < 50 || l < 50) return;

    // Track TE (shortest pulse)
    uint32_t shorter = h < l ? h : l;
    if(shorter < 500) {
        app->rx_te_sum += shorter;
        app->rx_te_n++;
    }

    // PWM decode: long HIGH = 1, short HIGH = 0
    if(app->rx_bit_count < 128) {
        uint32_t te_est = app->rx_te_n > 0 ? app->rx_te_sum / app->rx_te_n : 200;
        uint32_t threshold = te_est * 2;
        app->rx_bits[app->rx_bit_count++] = (h > threshold) ? 1 : 0;
    }
}

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
        // Sync
        furi_hal_gpio_write(&gpio_cc1101_g0, true);
        furi_delay_us(te);
        furi_hal_gpio_write(&gpio_cc1101_g0, false);
        furi_delay_us(te * 31);
        // Data (PWM)
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
    // Simple insertion sort (max 32 elements)
    for(uint8_t i = 1; i < app->signal_count; i++) {
        NRSignal tmp = app->signals[i];
        int8_t j = i - 1;
        while(j >= 0) {
            bool swap;
            if(app->sort == NRSortHits)
                swap = app->signals[j].hits < tmp.hits;
            else
                swap = app->signals[j].last_seen < tmp.last_seen;
            if(!swap) break;
            app->signals[j + 1] = app->signals[j];
            j--;
        }
        app->signals[j + 1] = tmp;
    }
}

// ============== Drawing ==============

static void nr_draw(Canvas* canvas, void* ctx) {
    NRApp* app = ctx;
    canvas_clear(canvas);
    char buf[64];

    if(app->view == NRViewDash) {
        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "Neighborhood  %d signals", app->signal_count);
        canvas_draw_str(canvas, 0, 11, buf);
        canvas_set_font(canvas, FontSecondary);

        if(app->signal_count == 0) {
            canvas_draw_str(canvas, 4, 28, "No signals captured yet.");
            canvas_draw_str(canvas, 4, 40, "OK: start scanning");
        } else {
            // Count per protocol
            uint8_t counts[NRProtoCount] = {0};
            for(uint8_t i = 0; i < app->signal_count; i++)
                counts[app->signals[i].proto]++;

            uint8_t y = 22;
            for(uint8_t p = 0; p < NRProtoCount; p++) {
                if(counts[p] == 0) continue;
                snprintf(buf, sizeof(buf), "%s %s  %d signals",
                    nr_proto_icon[p], nr_proto_name[p], counts[p]);
                canvas_draw_str(canvas, 2, y, buf);
                y += 9;
                if(y > 54) break;
            }
        }
        canvas_draw_str(canvas, 0, 63, "OK:scan  U/D:browse  Bk:exit");

    } else if(app->view == NRViewScan) {
        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "Scan 433.92  %s  %dsig",
            app->sort == NRSortHits ? "HITS" : "RECENT", app->signal_count);
        canvas_draw_str(canvas, 0, 11, buf);
        canvas_set_font(canvas, FontSecondary);

        uint8_t start = app->sel > 4 ? app->sel - 4 : 0;
        for(uint8_t i = start; i < app->signal_count && (i - start) < 5; i++) {
            uint8_t y = 21 + (i - start) * 9;
            NRSignal* s = &app->signals[i];
            if(i == app->sel) {
                canvas_draw_box(canvas, 0, y - 7, 128, 9);
                canvas_set_color(canvas, ColorWhite);
            }
            snprintf(buf, sizeof(buf), "%3lux %s %-9s %s",
                (unsigned long)s->hits, nr_proto_icon[s->proto],
                nr_proto_name[s->proto], s->info);
            // Truncate to fit screen
            buf[42] = 0;
            canvas_draw_str(canvas, 0, y, buf);
            canvas_set_color(canvas, ColorBlack);
        }
        canvas_draw_str(canvas, 0, 63, "OK:detail L/R:sort Bk:dash");

    } else if(app->view == NRViewDetail) {
        if(app->sel >= app->signal_count) { app->view = NRViewScan; return; }
        NRSignal* s = &app->signals[app->sel];

        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "%s %s  %lux",
            nr_proto_icon[s->proto], nr_proto_name[s->proto], (unsigned long)s->hits);
        canvas_draw_str(canvas, 0, 11, buf);

        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 0, 21, s->info);

        snprintf(buf, sizeof(buf), "TE:%d  Bits:%d", s->te, s->bit_count);
        canvas_draw_str(canvas, 0, 31, buf);

        // Show raw hex
        char hex[48] = {0};
        for(uint8_t h = 0; h < s->raw_len && h < 8; h++)
            snprintf(hex + h * 3, 4, "%02X ", s->raw_frame[h]);
        canvas_draw_str(canvas, 0, 41, hex);

        // Protocol info
        if(nr_proto_replayable[s->proto]) {
            canvas_draw_str(canvas, 0, 51, "Replayable: YES");
            canvas_draw_str(canvas, 0, 63, "OK:TRANSMIT  Bk:list");
        } else if(s->proto == NRProtoFSK) {
            canvas_draw_str(canvas, 0, 51, "FSK signal — camp on FM476");
            canvas_draw_str(canvas, 0, 63, "Bk:list");
        } else if(s->proto == NRProtoHoneywell) {
            canvas_draw_str(canvas, 0, 51, "Alarm sensor (listen only)");
            canvas_draw_str(canvas, 0, 63, "Bk:list");
        } else if(s->proto == NRProtoKeeloq) {
            canvas_draw_str(canvas, 0, 51, "Rolling code (encrypted)");
            canvas_draw_str(canvas, 0, 63, "Bk:list");
        } else {
            canvas_draw_str(canvas, 0, 51, "Unknown protocol");
            canvas_draw_str(canvas, 0, 63, "Bk:list");
        }
    }
}

static void nr_input(InputEvent* event, void* ctx) {
    NRApp* app = ctx;
    furi_message_queue_put(app->event_queue, event, FuriWaitForever);
}

// ============== Process received frame ==============

static void nr_process_frame(NRApp* app) {
    if(!app->rx_frame_ready) return;
    app->rx_frame_ready = false;

    uint16_t te = app->rx_frame_te;
    uint16_t bits = app->rx_frame_bits;
    uint8_t len = app->rx_frame_len;

    NRProto proto = nr_classify(te, bits, app->rx_frame_data, len);

    // Find existing or allocate new slot
    int8_t idx = nr_find_match(app, proto, app->rx_frame_data, len);
    if(idx >= 0) {
        app->signals[idx].hits++;
        app->signals[idx].last_seen = app->tick;
        return; // Already known, just bump counter
    }

    // New signal
    uint8_t slot = nr_find_slot(app);
    NRSignal* s = &app->signals[slot];
    memset(s, 0, sizeof(NRSignal));
    s->proto = proto;
    s->te = te;
    s->bit_count = bits;
    s->hits = 1;
    s->last_seen = app->tick;
    s->raw_len = len;
    memcpy(s->raw_frame, app->rx_frame_data, len);
    nr_decode_info(s);

    if(slot >= app->signal_count && app->signal_count < NR_MAX_SIGNALS)
        app->signal_count++;

    // Autosave
    nr_autosave(app, s);
    notification_message(app->notifications, &sequence_blink_cyan_10);
}

// ============== Main ==============

int32_t neighborhood_remote_app(void* p) {
    UNUSED(p);
    NRApp* app = malloc(sizeof(NRApp));
    memset(app, 0, sizeof(NRApp));

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
                } else if(event.key == InputKeyUp && app->sel > 0) {
                    app->sel--;
                } else if(event.key == InputKeyDown && app->sel + 1 < app->signal_count) {
                    app->sel++;
                }

            } else if(app->view == NRViewScan) {
                if(event.key == InputKeyBack) {
                    nr_stop_rx(app);
                    app->view = NRViewDash;
                } else if(event.key == InputKeyOk && app->signal_count > 0) {
                    app->view = NRViewDetail;
                } else if(event.key == InputKeyUp && app->sel > 0) {
                    app->sel--;
                } else if(event.key == InputKeyDown && app->sel + 1 < app->signal_count) {
                    app->sel++;
                } else if(event.key == InputKeyLeft || event.key == InputKeyRight) {
                    app->sort = (app->sort == NRSortHits) ? NRSortRecent : NRSortHits;
                    nr_sort_signals(app);
                    app->sel = 0;
                }

            } else if(app->view == NRViewDetail) {
                if(event.key == InputKeyBack) {
                    app->view = NRViewScan;
                } else if(event.key == InputKeyOk && app->sel < app->signal_count) {
                    NRSignal* s = &app->signals[app->sel];
                    if(nr_proto_replayable[s->proto]) {
                        notification_message(app->notifications, &sequence_blink_magenta_100);
                        nr_transmit(app, s);
                        notification_message(app->notifications, &sequence_blink_green_100);
                    }
                }
            }
        }

tick:
        app->tick++;
        nr_process_frame(app);

        // Re-sort periodically in scan mode
        if(app->view == NRViewScan && (app->tick % 40) == 0)
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
