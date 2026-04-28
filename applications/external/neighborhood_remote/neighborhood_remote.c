/*
 * Neighborhood Remote — Universal 433.92 MHz signal learner & replayer
 * Decodes Honeywell alarm, EV1527, Keeloq (listen), fixed-code remotes.
 * Based on Dooya Remote architecture (SubGhzWorker raw pair callback).
 */
#include "neighborhood_remote.h"
#define TAG "Neighborhood"

// ============== Category detection ==============
static NRCategory nr_detect_category(uint16_t te) {
    if(te >= 220 && te <= 360) return NRCatKeeloq;
    if(te >= 130 && te <= 160) return NRCatAlarm;
    if(te >= 180 && te <= 210) return NRCatRemote;
    if(te >= 105 && te <= 130) return NRCatEV1527;
    if(te >= 65 && te <= 95) return NRCatWeather;
    return NRCatUnknown;
}

// ============== Protocol decode helpers ==============
// Decode Honeywell Manchester data into info string
static void nr_decode_honeywell(uint8_t* data, uint16_t bits, char* info, size_t info_sz) {
    // Honeywell: FF FE + 4-bit ch + 20-bit serial + 8-bit event + 16-bit CRC
    // We get raw PWM bits from BinRAW, need Manchester decode first
    // For now show raw hex — full Manchester decode needs pulse-level access
    if(bits >= 48) {
        uint32_t serial = ((data[2] & 0x0F) << 16) | (data[3] << 8) | data[4];
        uint8_t event = data[5];
        snprintf(info, info_sz, "Sn:%lu %s%s%s%s",
            (unsigned long)serial,
            (event & 0x80) ? "OPEN " : "closed ",
            (event & 0x40) ? "TAMPER " : "",
            (event & 0x08) ? "LOBAT " : "",
            (event & 0x04) ? "HB" : "");
    } else {
        snprintf(info, info_sz, "Alarm TE~143 %db", bits);
    }
}

// Decode EV1527: 20-bit address + 4-bit command from 24 data bits
static void nr_decode_ev1527(uint8_t* data, uint16_t bits, char* info, size_t info_sz) {
    if(bits >= 24) {
        uint32_t val = (data[0] << 16) | (data[1] << 8) | data[2];
        uint32_t addr = (val >> 4) & 0xFFFFF;
        uint8_t cmd = val & 0x0F;
        snprintf(info, info_sz, "Addr:%05lX Cmd:%X", (unsigned long)addr, cmd);
    } else {
        snprintf(info, info_sz, "EV1527 %db", bits);
    }
}

// Decode Keeloq: 28-bit serial + 4-bit button from tail of 66-bit frame
static void nr_decode_keeloq(uint8_t* data, uint16_t bits, char* info, size_t info_sz) {
    if(bits >= 64) {
        // Serial is bits 32-59 (28 bits), button is bits 60-63 (4 bits)
        uint32_t serial = ((data[4] & 0x0F) << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
        serial >>= 4;
        uint8_t btn = data[7] & 0x0F;
        snprintf(info, info_sz, "Sn:%07lX Btn:%X (encrypted)", (unsigned long)serial, btn);
    } else {
        snprintf(info, info_sz, "Keeloq %db", bits);
    }
}

static void nr_decode_info(NRCategory cat, uint8_t* data, uint16_t bits, char* info, size_t sz) {
    switch(cat) {
    case NRCatAlarm: nr_decode_honeywell(data, bits, info, sz); break;
    case NRCatEV1527: nr_decode_ev1527(data, bits, info, sz); break;
    case NRCatKeeloq: nr_decode_keeloq(data, bits, info, sz); break;
    case NRCatRemote:
        snprintf(info, sz, "Fixed-code TE~194 (replayable)");
        break;
    case NRCatWeather:
        snprintf(info, sz, "Weather sensor (passive)");
        break;
    default: {
        // Show first 4 hex bytes
        char hex[16] = {0};
        uint8_t len = (bits + 7) / 8;
        for(uint8_t i = 0; i < len && i < 4; i++)
            snprintf(hex + i * 3, 4, "%02X ", data[i]);
        snprintf(info, sz, "Raw: %s(%db)", hex, bits);
        break;
    }
    }
}

// ============== File I/O ==============
static void nr_save(NRApp* app) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_DIR);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, NR_SAVE_FILE)) {
        flipper_format_write_header_cstr(ff, "Neighborhood Devices", 1);
        uint32_t cnt = app->device_count;
        flipper_format_write_uint32(ff, "Count", &cnt, 1);
        for(uint8_t d = 0; d < app->device_count; d++) {
            NRDevice* dev = &app->devices[d];
            uint32_t hdr[2] = {dev->category, dev->btn_count};
            flipper_format_write_string_cstr(ff, "Device", dev->name);
            flipper_format_write_uint32(ff, "Hdr", hdr, 2);
            for(uint8_t b = 0; b < dev->btn_count; b++) {
                NRButton* btn = &dev->buttons[b];
                uint32_t binfo[2] = {btn->te, btn->bit_count};
                flipper_format_write_string_cstr(ff, "Btn", btn->name);
                flipper_format_write_uint32(ff, "Info", binfo, 2);
                flipper_format_write_hex(ff, "Data", btn->data, btn->data_len);
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
            for(uint32_t d = 0; d < cnt; d++) {
                NRDevice* dev = &app->devices[d];
                if(!flipper_format_read_string(ff, "Device", str)) break;
                snprintf(dev->name, NR_MAX_NAME, "%s", furi_string_get_cstr(str));
                uint32_t hdr[2] = {0};
                flipper_format_read_uint32(ff, "Hdr", hdr, 2);
                dev->category = hdr[0] < NRCatCount ? hdr[0] : NRCatUnknown;
                dev->btn_count = hdr[1] > NR_MAX_BUTTONS ? NR_MAX_BUTTONS : hdr[1];
                for(uint8_t b = 0; b < dev->btn_count; b++) {
                    NRButton* btn = &dev->buttons[b];
                    if(!flipper_format_read_string(ff, "Btn", str)) break;
                    snprintf(btn->name, NR_MAX_NAME, "%s", furi_string_get_cstr(str));
                    uint32_t bi[2] = {0};
                    flipper_format_read_uint32(ff, "Info", bi, 2);
                    btn->te = bi[0];
                    btn->bit_count = bi[1];
                    btn->data_len = (btn->bit_count + 7) / 8;
                    if(btn->data_len > 32) btn->data_len = 32;
                    flipper_format_read_hex(ff, "Data", btn->data, btn->data_len);
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

// ============== Radio RX ==============
static void nr_rx_callback(void* ctx, bool level, uint32_t duration) {
    NRApp* app = ctx;
    if(app->rx_got_signal) return; // already captured

    // Store raw pulses
    if(app->rx_pulse_count < 512) {
        app->rx_pulses[app->rx_pulse_count++] = level ? (int32_t)duration : -(int32_t)duration;
    }

    // Simple signal detection: look for a burst of pulses after silence
    if(!level && duration > 5000) {
        // Long gap = potential end of frame
        if(app->rx_pulse_count > 20) {
            app->rx_got_signal = true;
        }
    }

    // Timeout: if we have >200 pulses, consider it captured
    if(app->rx_pulse_count >= 200) {
        app->rx_got_signal = true;
    }
}

static void nr_start_rx(NRApp* app) {
    if(app->rx_active) return;
    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio_device, 433920000);
    subghz_worker_set_pair_callback(app->worker, (SubGhzWorkerPairCallback)nr_rx_callback);
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

// ============== Analyze captured pulses ==============
static bool nr_analyze(NRApp* app) {
    if(app->rx_pulse_count < 10) return false;

    // Find TE: median of short pulse durations
    uint32_t shorts[256];
    uint16_t sc = 0;
    for(uint16_t i = 0; i < app->rx_pulse_count && sc < 256; i++) {
        uint32_t d = app->rx_pulses[i] > 0 ? app->rx_pulses[i] : -app->rx_pulses[i];
        if(d > 50 && d < 500) shorts[sc++] = d;
    }
    if(sc < 4) return false;

    // Simple average for TE
    uint32_t sum = 0;
    for(uint16_t i = 0; i < sc; i++) sum += shorts[i];
    uint16_t te = sum / sc;
    if(te < 50) return false;

    // PWM decode: short high + long low = 0, long high + short low = 1
    uint16_t threshold = te * 2;
    uint8_t bits[256];
    uint16_t bit_count = 0;

    for(uint16_t i = 0; i + 1 < app->rx_pulse_count && bit_count < 256; i += 2) {
        int32_t p1 = app->rx_pulses[i];
        int32_t p2 = app->rx_pulses[i + 1];
        if(p1 <= 0 || p2 >= 0) continue;
        uint32_t h = p1, l = -p2;
        if(l > te * 15 || h > te * 15) continue; // skip sync gaps
        bits[bit_count++] = (h > threshold) ? 1 : 0;
    }
    if(bit_count < 8) return false;

    app->learn_te = te;
    app->learn_bits = bit_count;
    app->learn_data_len = (bit_count + 7) / 8;
    if(app->learn_data_len > 32) app->learn_data_len = 32;
    memset(app->learn_data, 0, 32);
    for(uint16_t i = 0; i < bit_count && i < 256; i++) {
        if(bits[i]) app->learn_data[i / 8] |= (1 << (7 - (i % 8)));
    }
    app->learn_cat = nr_detect_category(te);
    nr_decode_info(app->learn_cat, app->learn_data, app->learn_bits, app->learn_info, sizeof(app->learn_info));
    return true;
}

// ============== TX (for replayable devices) ==============
static void nr_transmit(NRApp* app, NRButton* btn) {
    if(btn->data_len == 0) return;
    bool was_rx = app->rx_active;
    if(was_rx) nr_stop_rx(app);

    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio_device, 433920000);

    uint16_t te = btn->te;
    uint16_t te3 = te * 3;

    for(int rep = 0; rep < 5; rep++) {
        subghz_devices_set_tx(app->radio_device);
        // Sync pulse
        furi_hal_gpio_write(&gpio_cc1101_g0, true);
        furi_delay_us(te);
        furi_hal_gpio_write(&gpio_cc1101_g0, false);
        furi_delay_us(te * 31);
        // Data bits (PWM)
        for(uint16_t i = 0; i < btn->bit_count; i++) {
            uint8_t bit = (btn->data[i / 8] >> (7 - (i % 8))) & 1;
            furi_hal_gpio_write(&gpio_cc1101_g0, true);
            furi_delay_us(bit ? te3 : te);
            furi_hal_gpio_write(&gpio_cc1101_g0, false);
            furi_delay_us(bit ? te : te3);
        }
        // End gap
        furi_hal_gpio_write(&gpio_cc1101_g0, false);
        subghz_devices_idle(app->radio_device);
        furi_delay_ms(8);
    }

    if(was_rx) nr_start_rx(app);
}

// ============== Drawing ==============
static void nr_draw(Canvas* canvas, void* ctx) {
    NRApp* app = ctx;
    canvas_clear(canvas);
    char buf[64];

    if(app->current_view == NRViewList) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 0, 11, "Neighborhood 433.92");
        canvas_set_font(canvas, FontSecondary);

        if(app->device_count == 0) {
            canvas_draw_str(canvas, 4, 26, "No devices learned yet.");
            canvas_draw_str(canvas, 4, 38, "OK: learn a signal");
        } else {
            uint8_t start = app->sel_device > 4 ? app->sel_device - 4 : 0;
            for(uint8_t i = start; i < app->device_count && (i - start) < 5; i++) {
                uint8_t y = 22 + (i - start) * 9;
                NRDevice* dev = &app->devices[i];
                if(i == app->sel_device) {
                    canvas_draw_box(canvas, 0, y - 7, 128, 9);
                    canvas_set_color(canvas, ColorWhite);
                }
                snprintf(buf, sizeof(buf), "%s [%s] %db",
                    dev->name, nr_cat_names[dev->category], dev->btn_count);
                canvas_draw_str(canvas, 2, y, buf);
                canvas_set_color(canvas, ColorBlack);
            }
        }
        canvas_draw_str(canvas, 0, 63, "OK:open U/D:sel LongOK:learn");

    } else if(app->current_view == NRViewDetail) {
        NRDevice* dev = &app->devices[app->sel_device];
        canvas_set_font(canvas, FontPrimary);
        snprintf(buf, sizeof(buf), "%s [%s]", dev->name, nr_cat_names[dev->category]);
        canvas_draw_str(canvas, 0, 11, buf);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 0, 20, nr_cat_desc[dev->category]);

        for(uint8_t b = 0; b < dev->btn_count && b < 3; b++) {
            uint8_t y = 30 + b * 10;
            NRButton* btn = &dev->buttons[b];
            if(b == app->sel_button) {
                canvas_draw_box(canvas, 0, y - 7, 128, 10);
                canvas_set_color(canvas, ColorWhite);
            }
            // Show decoded info
            char info[48];
            nr_decode_info(dev->category, btn->data, btn->bit_count, info, sizeof(info));
            snprintf(buf, sizeof(buf), "%s: %s", btn->name, info);
            canvas_draw_str(canvas, 2, y, buf);
            canvas_set_color(canvas, ColorBlack);
        }

        bool replayable = (dev->category == NRCatRemote || dev->category == NRCatEV1527 || dev->category == NRCatUnknown);
        if(replayable)
            canvas_draw_str(canvas, 0, 63, "OK:TX U/D:sel LongOK:learn");
        else
            canvas_draw_str(canvas, 0, 63, "U/D:sel LongOK:learn Bk:list");

    } else if(app->current_view == NRViewLearn) {
        canvas_set_font(canvas, FontPrimary);
        if(!app->rx_got_signal) {
            canvas_draw_str(canvas, 0, 11, "Listening...");
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 4, 26, "Press a button on the");
            canvas_draw_str(canvas, 4, 36, "target device now.");
            snprintf(buf, sizeof(buf), "Pulses: %d", app->rx_pulse_count);
            canvas_draw_str(canvas, 4, 48, buf);
            canvas_draw_str(canvas, 0, 63, "Back: cancel");
        } else {
            canvas_draw_str(canvas, 0, 11, "Signal captured!");
            canvas_set_font(canvas, FontSecondary);
            snprintf(buf, sizeof(buf), "TE=%d  Bits=%d", app->learn_te, app->learn_bits);
            canvas_draw_str(canvas, 4, 24, buf);
            snprintf(buf, sizeof(buf), "Type: %s", nr_cat_names[app->learn_cat]);
            canvas_draw_str(canvas, 4, 34, buf);
            canvas_draw_str(canvas, 4, 44, app->learn_info);
            canvas_draw_str(canvas, 0, 63, "OK:save Back:retry");
        }
    }
}

static void nr_input(InputEvent* event, void* ctx) {
    NRApp* app = ctx;
    furi_message_queue_put(app->event_queue, event, FuriWaitForever);
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

    bool running = true;
    InputEvent event;
    while(running) {
        if(furi_message_queue_get(app->event_queue, &event, 100) == FuriStatusOk) {
            if(event.type != InputTypeShort && event.type != InputTypeLong) goto redraw;

            if(app->current_view == NRViewList) {
                if(event.key == InputKeyBack) {
                    running = false;
                } else if(event.key == InputKeyUp && app->sel_device > 0) {
                    app->sel_device--;
                } else if(event.key == InputKeyDown && app->sel_device + 1 < app->device_count) {
                    app->sel_device++;
                } else if(event.key == InputKeyOk && event.type == InputTypeShort && app->device_count > 0) {
                    app->sel_button = 0;
                    app->current_view = NRViewDetail;
                } else if(event.key == InputKeyOk) {
                    // Long OK or OK when empty → learn
                    app->rx_got_signal = false;
                    app->rx_pulse_count = 0;
                    app->learn_info[0] = 0;
                    app->current_view = NRViewLearn;
                    nr_start_rx(app);
                }

            } else if(app->current_view == NRViewDetail) {
                NRDevice* dev = &app->devices[app->sel_device];
                if(event.key == InputKeyBack) {
                    app->current_view = NRViewList;
                } else if(event.key == InputKeyUp && app->sel_button > 0) {
                    app->sel_button--;
                } else if(event.key == InputKeyDown && app->sel_button + 1 < dev->btn_count) {
                    app->sel_button++;
                } else if(event.key == InputKeyOk && event.type == InputTypeShort && dev->btn_count > 0) {
                    bool replayable = (dev->category == NRCatRemote || dev->category == NRCatEV1527 || dev->category == NRCatUnknown);
                    if(replayable) {
                        notification_message(app->notifications, &sequence_blink_magenta_100);
                        nr_transmit(app, &dev->buttons[app->sel_button]);
                        notification_message(app->notifications, &sequence_blink_green_100);
                    }
                } else if(event.key == InputKeyOk && event.type == InputTypeLong) {
                    app->rx_got_signal = false;
                    app->rx_pulse_count = 0;
                    app->learn_info[0] = 0;
                    app->current_view = NRViewLearn;
                    nr_start_rx(app);
                }

            } else if(app->current_view == NRViewLearn) {
                if(event.key == InputKeyBack) {
                    nr_stop_rx(app);
                    app->current_view = app->device_count > 0 ? NRViewDetail : NRViewList;
                } else if(event.key == InputKeyOk && app->rx_got_signal) {
                    nr_stop_rx(app);
                    // Add to current device or create new
                    NRDevice* target = NULL;
                    if(app->sel_device < app->device_count) {
                        target = &app->devices[app->sel_device];
                    }
                    if(!target && app->device_count < NR_MAX_DEVICES) {
                        target = &app->devices[app->device_count];
                        memset(target, 0, sizeof(NRDevice));
                        snprintf(target->name, NR_MAX_NAME, "%s-%d",
                            nr_cat_names[app->learn_cat], app->device_count + 1);
                        target->category = app->learn_cat;
                        app->sel_device = app->device_count;
                        app->device_count++;
                    }
                    if(target && target->btn_count < NR_MAX_BUTTONS) {
                        NRButton* btn = &target->buttons[target->btn_count];
                        snprintf(btn->name, NR_MAX_NAME, "Sig%d", target->btn_count + 1);
                        btn->te = app->learn_te;
                        btn->bit_count = app->learn_bits;
                        btn->data_len = app->learn_data_len;
                        memcpy(btn->data, app->learn_data, btn->data_len);
                        target->btn_count++;
                    }
                    nr_save(app);
                    app->current_view = NRViewDetail;
                    notification_message(app->notifications, &sequence_success);
                }
            }
        }

        // Check if learn captured a signal
        if(app->current_view == NRViewLearn && app->rx_got_signal && app->learn_info[0] == 0) {
            nr_analyze(app);
        }

redraw:
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
