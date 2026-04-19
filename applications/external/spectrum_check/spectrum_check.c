#include "spectrum_check.h"
#include <stdlib.h>
#include <string.h>
#include <float_tools.h>

extern const SubGhzProtocolRegistry subghz_protocol_registry;

// ============== Spacing/band config ==============

static const uint32_t sc_spacing[] = {196078, 39215, 784313};
static const uint32_t sc_step[] = {5000, 1000, 20000};
static const char* sc_width_names[] = {"Wide", "Narrow", "UltraWide"};
static const char* sc_view_names[] = {"SPECTRUM", "FREQ ANALYZER", "DECODER", "WAVEFORM"};
static const char* sc_sort_names[] = {"Count", "RSSI", "Freq", "Recent"};
static const char* sc_mod_names[] = {"OOK650", "OOK270", "2FSK"};

// ============== Log sort ==============

static void sc_log_sort(SCLogEntry* log, uint8_t size, SCLogSort mode) {
    for(uint8_t i = 1; i < size; i++) {
        SCLogEntry tmp = log[i];
        int8_t j = i - 1;
        while(j >= 0) {
            bool swap;
            switch(mode) {
            case SCLogSortCount: swap = log[j].count < tmp.count; break;
            case SCLogSortRSSI: swap = log[j].rssi_max < tmp.rssi_max; break;
            case SCLogSortFreq: swap = log[j].frequency > tmp.frequency; break;
            default: swap = log[j].seq < tmp.seq; break;
            }
            if(!swap) break;
            log[j + 1] = log[j];
            j--;
        }
        log[j + 1] = tmp;
    }
}

static void sc_log_update(SpectrumCheckApp* app, uint32_t freq, float rssi) {
    if(!freq) return;
    uint8_t rssi_u8 = (uint8_t)(rssi - SC_RSSI_MIN);

    for(uint8_t i = 0; i < app->log_size; i++) {
        if(app->log[i].frequency == freq) {
            if(app->log[i].count < 255) app->log[i].count++;
            if(rssi_u8 > app->log[i].rssi_max) app->log[i].rssi_max = rssi_u8;
            app->log[i].seq = app->log_seq++;
            sc_log_sort(app->log, app->log_size, app->log_sort);
            return;
        }
    }
    if(app->log_size < SC_MAX_LOG) {
        app->log[app->log_size].frequency = freq;
        app->log[app->log_size].count = 1;
        app->log[app->log_size].rssi_max = rssi_u8;
        app->log[app->log_size].seq = app->log_seq++;
        app->log_size++;
        sc_log_sort(app->log, app->log_size, app->log_sort);
    }
}

// ============== Raw sample capture ==============

static void sc_raw_callback(bool level, uint32_t duration, void* context) {
    SpectrumCheckApp* app = context;
    if(app->raw_write_idx < SC_RAW_SAMPLES_MAX) {
        app->raw_samples[app->raw_write_idx].level = level;
        app->raw_samples[app->raw_write_idx].duration = duration;
        app->raw_write_idx++;
        if(app->raw_count < SC_RAW_SAMPLES_MAX) app->raw_count++;
    }
}

// ============== Spectrum worker ==============

static int32_t sc_spectrum_worker(void* ctx) {
    SpectrumCheckApp* app = ctx;

    while(app->worker_running) {
        if(app->current_view == SCViewSpectrum) {
            // Sweep channels
            uint32_t spacing = sc_spacing[app->width];
            uint32_t ch0 = app->frequency - (SC_NUM_CHANNELS / 2) * spacing;

            subghz_devices_idle(app->radio_device);
            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);

            float best_rssi = -200.0f;
            uint8_t best_ch = 0;

            for(uint8_t i = 0; i < SC_NUM_CHANNELS && app->worker_running; i++) {
                uint32_t f = ch0 + i * spacing;
                subghz_devices_set_frequency(app->radio_device, f);
                subghz_devices_set_rx(app->radio_device);
                furi_delay_us(300);
                float rssi = subghz_devices_get_rssi(app->radio_device);
                subghz_devices_idle(app->radio_device);

                furi_mutex_acquire(app->mutex, FuriWaitForever);
                int8_t db = (int8_t)rssi + 138; // normalize to 0-80 range
                if(db < 0) db = 0;
                if(db > 80) db = 80;
                app->channel_ss[i] = (uint8_t)db;
                if(rssi > best_rssi) {
                    best_rssi = rssi;
                    best_ch = i;
                }
                furi_mutex_release(app->mutex);
            }

            furi_mutex_acquire(app->mutex, FuriWaitForever);
            app->max_rssi = best_rssi;
            app->max_rssi_channel = best_ch;
            furi_mutex_release(app->mutex);

        } else if(app->current_view == SCViewFreqAnalyzer) {
            // Frequency analyzer: scan common frequencies for signals
            static const uint32_t scan_freqs[] = {
                300000000, 303875000, 310000000, 315000000, 318000000,
                390000000, 418000000, 433075000, 433420000, 433920000,
                434420000, 434775000, 438900000,
                868350000, 868950000, 915000000, 925000000,
            };
            static const uint8_t scan_count = sizeof(scan_freqs) / sizeof(scan_freqs[0]);

            subghz_devices_idle(app->radio_device);
            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);

            for(uint8_t i = 0; i < scan_count && app->worker_running; i++) {
                subghz_devices_set_frequency(app->radio_device, scan_freqs[i]);
                subghz_devices_set_rx(app->radio_device);
                furi_delay_us(500);
                float rssi = subghz_devices_get_rssi(app->radio_device);
                subghz_devices_idle(app->radio_device);

                furi_mutex_acquire(app->mutex, FuriWaitForever);
                if(rssi > app->trigger) {
                    app->signal_found = true;
                    app->detected_freq = scan_freqs[i];
                    app->detected_rssi = rssi;
                    sc_log_update(app, scan_freqs[i], rssi);
                }
                furi_mutex_release(app->mutex);
            }

        } else if(app->current_view == SCViewDecoder || app->current_view == SCViewWaveform) {
            // Capture raw pulses on current frequency
            if(!app->signal_found && app->detected_freq == 0) {
                furi_delay_ms(100);
                continue;
            }

            subghz_devices_idle(app->radio_device);
            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
            subghz_devices_set_frequency(app->radio_device, app->detected_freq ? app->detected_freq : app->frequency);

            furi_mutex_acquire(app->mutex, FuriWaitForever);
            app->raw_write_idx = 0;
            app->raw_count = 0;
            furi_mutex_release(app->mutex);

            subghz_devices_start_async_rx(app->radio_device, sc_raw_callback, app);
            furi_delay_ms(500); // Capture for 500ms
            subghz_devices_stop_async_rx(app->radio_device);
            subghz_devices_idle(app->radio_device);

            // Try to decode
            if(app->raw_count > 10) {
                furi_mutex_acquire(app->mutex, FuriWaitForever);
                // Analyze raw data for basic stats
                uint32_t total_dur = 0;
                uint32_t min_pulse = UINT32_MAX;
                uint16_t high_count = 0;
                for(uint16_t i = 0; i < app->raw_count; i++) {
                    total_dur += app->raw_samples[i].duration;
                    if(app->raw_samples[i].level) high_count++;
                    if(app->raw_samples[i].duration < min_pulse && app->raw_samples[i].duration > 50) {
                        min_pulse = app->raw_samples[i].duration;
                    }
                }
                if(!app->decoded.decoded && app->raw_count > 20) {
                    snprintf(app->decoded.protocol_name, sizeof(app->decoded.protocol_name), "Unknown");
                    snprintf(app->decoded.info_str, sizeof(app->decoded.info_str),
                        "Pulses:%d Hi:%d\nDur:%ldms Te~%ldus\nRate~%ldHz",
                        app->raw_count, high_count,
                        total_dur / 1000,
                        min_pulse < UINT32_MAX ? min_pulse : 0,
                        min_pulse < UINT32_MAX && min_pulse > 0 ? 1000000 / min_pulse : 0);
                    app->decoded.decoded = true;
                }
                furi_mutex_release(app->mutex);
            }
        }

        furi_delay_ms(50);
    }

    subghz_devices_idle(app->radio_device);
    return 0;
}

// ============== Drawing ==============

static void sc_draw_spectrum(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[32];

    // Draw bars
    for(uint8_t i = 0; i < SC_NUM_CHANNELS; i++) {
        uint8_t h = app->channel_ss[i] * 40 / 80;
        if(h > 0) {
            canvas_draw_line(canvas, SC_FREQ_START_X + i, SC_FREQ_BOTTOM_Y, SC_FREQ_START_X + i, SC_FREQ_BOTTOM_Y - h);
        }
    }

    // Peak marker
    if(app->max_rssi > -90.0f) {
        uint8_t px = SC_FREQ_START_X + app->max_rssi_channel;
        canvas_draw_line(canvas, px, 10, px, SC_FREQ_BOTTOM_Y);
        snprintf(buf, sizeof(buf), "%.1f", (double)app->max_rssi);
        canvas_draw_str(canvas, px + 2, 18, buf);
    }

    // Scale
    uint32_t spacing = sc_spacing[app->width];
    uint32_t left = app->frequency - (SC_NUM_CHANNELS / 2) * spacing;
    uint32_t right = app->frequency + (SC_NUM_CHANNELS / 2) * spacing;
    snprintf(buf, sizeof(buf), "%ld.%01ld", left / 1000000, (left / 100000) % 10);
    canvas_draw_str(canvas, 0, 62, buf);
    snprintf(buf, sizeof(buf), "%ld.%01ld", right / 1000000, (right / 100000) % 10);
    canvas_draw_str(canvas, 90, 62, buf);

    // Width mode
    canvas_draw_str(canvas, 50, 62, sc_width_names[app->width]);
}

static void sc_draw_freq_analyzer(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];

    // Detected frequency
    canvas_set_font(canvas, FontBigNumbers);
    if(app->detected_freq > 0) {
        snprintf(buf, sizeof(buf), "%03ld.%03ld",
            app->detected_freq / 1000000 % 1000, app->detected_freq / 1000 % 1000);
        if(app->signal_found) {
            canvas_draw_box(canvas, 2, 10, 124, 19);
            canvas_set_color(canvas, ColorWhite);
        }
        canvas_draw_str(canvas, 6, 26, buf);
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_str(canvas, 6, 26, "----.---");
    }

    // RSSI bar
    canvas_set_font(canvas, FontSecondary);
    snprintf(buf, sizeof(buf), "T:%.0f", (double)app->trigger);
    canvas_draw_str(canvas, 0, 36, buf);

    // Log list
    canvas_set_font(canvas, FontKeyboard);
    uint8_t y = 42;
    for(uint8_t i = 0; i < 3 && (app->log_scroll + i) < app->log_size; i++) {
        SCLogEntry* e = &app->log[app->log_scroll + i];
        snprintf(buf, sizeof(buf), "%03ld.%03ld x%d",
            e->frequency / 1000000 % 1000, e->frequency / 1000 % 1000, e->count);
        canvas_draw_str(canvas, 0, y + i * 9, buf);
        // Mini RSSI bar
        uint8_t bar = e->rssi_max > 40 ? 40 : e->rssi_max;
        for(uint8_t b = 0; b < bar; b++) {
            if(b % 4) canvas_draw_dot(canvas, 85 + b, y + i * 9 - 3);
        }
    }
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 85, 36, sc_sort_names[app->log_sort]);
}

static void sc_draw_decoder(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];

    canvas_set_font(canvas, FontSecondary);
    if(app->detected_freq > 0) {
        snprintf(buf, sizeof(buf), "%ld.%03ld MHz  %s",
            app->detected_freq / 1000000, (app->detected_freq / 1000) % 1000,
            sc_mod_names[app->modulation]);
        canvas_draw_str(canvas, 0, 18, buf);
    }

    if(app->decoded.decoded) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 0, 30, app->decoded.protocol_name);
        canvas_set_font(canvas, FontSecondary);

        // Draw multi-line info
        const char* p = app->decoded.info_str;
        uint8_t y = 40;
        while(*p && y < 64) {
            const char* nl = strchr(p, '\n');
            uint8_t len = nl ? (uint8_t)(nl - p) : strlen(p);
            if(len > 30) len = 30;
            char line[32];
            memcpy(line, p, len);
            line[len] = 0;
            canvas_draw_str(canvas, 0, y, line);
            y += 10;
            p += len;
            if(*p == '\n') p++;
        }
    } else {
        canvas_draw_str(canvas, 0, 36, "Waiting for signal...");
        snprintf(buf, sizeof(buf), "Samples: %d", app->raw_count);
        canvas_draw_str(canvas, 0, 48, buf);
    }
}

static void sc_draw_waveform(Canvas* canvas, SpectrumCheckApp* app) {
    if(app->raw_count == 0) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 10, 32, "No signal captured yet");
        return;
    }

    uint8_t zoom = app->waveform_zoom ? app->waveform_zoom : 1;
    uint16_t x = 0;
    uint16_t idx = app->waveform_scroll;

    while(x < 128 && idx < app->raw_count) {
        uint8_t pixels = (app->raw_samples[idx].duration / (100 / zoom));
        if(pixels == 0) pixels = 1;
        if(pixels > 128 - x) pixels = 128 - x;

        uint8_t y = app->raw_samples[idx].level ? 15 : 45;
        canvas_draw_line(canvas, x, y, x + pixels, y);

        // Vertical transition line
        if(idx > app->waveform_scroll) {
            uint8_t prev_y = app->raw_samples[idx - 1].level ? 15 : 45;
            if(prev_y != y) canvas_draw_line(canvas, x, 15, x, 45);
        }

        x += pixels;
        idx++;
    }

    // Info bar
    canvas_set_font(canvas, FontSecondary);
    char buf[32];
    snprintf(buf, sizeof(buf), "%d/%d z:%d", app->waveform_scroll, app->raw_count, zoom);
    canvas_draw_str(canvas, 0, 62, buf);
}

static void sc_draw_callback(Canvas* canvas, void* ctx) {
    SpectrumCheckApp* app = ctx;
    furi_mutex_acquire(app->mutex, FuriWaitForever);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    // Header
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 0, 7, sc_view_names[app->current_view]);

    // Center freq
    char buf[20];
    snprintf(buf, sizeof(buf), "%ld.%02ld",
        app->frequency / 1000000, (app->frequency / 10000) % 100);
    canvas_draw_str(canvas, 80, 7, buf);

    // Draw active view
    switch(app->current_view) {
    case SCViewSpectrum: sc_draw_spectrum(canvas, app); break;
    case SCViewFreqAnalyzer: sc_draw_freq_analyzer(canvas, app); break;
    case SCViewDecoder: sc_draw_decoder(canvas, app); break;
    case SCViewWaveform: sc_draw_waveform(canvas, app); break;
    default: break;
    }

    furi_mutex_release(app->mutex);
}

// ============== Input ==============

static void sc_input_callback(InputEvent* event, void* ctx) {
    SpectrumCheckApp* app = ctx;
    furi_message_queue_put(app->event_queue, event, FuriWaitForever);
}

static void sc_handle_input(SpectrumCheckApp* app, InputEvent* event) {
    if(event->type != InputTypeShort && event->type != InputTypeRepeat) return;

    furi_mutex_acquire(app->mutex, FuriWaitForever);

    switch(event->key) {
    case InputKeyUp:
        if(app->current_view == 0)
            app->current_view = SCViewCount - 1;
        else
            app->current_view--;
        // Reset decoder when switching to it
        if(app->current_view == SCViewDecoder) {
            app->decoded.decoded = false;
            app->raw_count = 0;
            app->raw_write_idx = 0;
        }
        break;

    case InputKeyDown:
        app->current_view = (app->current_view + 1) % SCViewCount;
        if(app->current_view == SCViewDecoder) {
            app->decoded.decoded = false;
            app->raw_count = 0;
            app->raw_write_idx = 0;
        }
        break;

    case InputKeyLeft:
        if(app->current_view == SCViewSpectrum) {
            app->frequency -= sc_step[app->width] * 1000;
            if(app->frequency < 300000000) app->frequency = 300000000;
        } else if(app->current_view == SCViewFreqAnalyzer) {
            app->trigger -= SC_TRIGGER_STEP;
            if(app->trigger < SC_RSSI_MIN) app->trigger = SC_RSSI_MIN;
        } else if(app->current_view == SCViewWaveform) {
            if(app->waveform_scroll > 0) app->waveform_scroll--;
        }
        break;

    case InputKeyRight:
        if(app->current_view == SCViewSpectrum) {
            app->frequency += sc_step[app->width] * 1000;
            if(app->frequency > 928000000) app->frequency = 928000000;
        } else if(app->current_view == SCViewFreqAnalyzer) {
            app->trigger += SC_TRIGGER_STEP;
            if(app->trigger > SC_RSSI_MAX) app->trigger = SC_RSSI_MAX;
        } else if(app->current_view == SCViewWaveform) {
            if(app->waveform_scroll + 1 < app->raw_count) app->waveform_scroll++;
        }
        break;

    case InputKeyOk:
        if(app->current_view == SCViewSpectrum) {
            // Cycle width
            app->width = (app->width + 1) % 3;
        } else if(app->current_view == SCViewFreqAnalyzer) {
            // Cycle sort
            app->log_sort = (app->log_sort + 1) % SCLogSortModes;
            sc_log_sort(app->log, app->log_size, app->log_sort);
            app->log_scroll = 0;
        } else if(app->current_view == SCViewDecoder) {
            // Cycle modulation
            app->modulation = (app->modulation + 1) % 3;
            app->decoded.decoded = false;
            app->raw_count = 0;
            app->raw_write_idx = 0;
        } else if(app->current_view == SCViewWaveform) {
            // Cycle zoom
            app->waveform_zoom = app->waveform_zoom >= 4 ? 1 : app->waveform_zoom + 1;
        }
        break;

    default:
        break;
    }

    furi_mutex_release(app->mutex);
}

// ============== App lifecycle ==============

int32_t spectrum_check_app(void* p) {
    UNUSED(p);

    SpectrumCheckApp* app = malloc(sizeof(SpectrumCheckApp));
    memset(app, 0, sizeof(SpectrumCheckApp));

    // Defaults
    app->frequency = 433920000;
    app->trigger = -85.0f;
    app->width = SCWidthWide;
    app->waveform_zoom = 1;
    app->running = true;

    // Init system
    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Init radio
    subghz_devices_init();
    app->radio_device = radio_device_loader_set(NULL, SubGhzRadioDeviceTypeInternal);

    // Init GUI
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, sc_draw_callback, app);
    view_port_input_callback_set(app->view_port, sc_input_callback, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    // Start worker
    app->worker_running = true;
    app->worker_thread = furi_thread_alloc_ex("SCWorker", 4096, sc_spectrum_worker, app);
    furi_thread_start(app->worker_thread);

    // Main loop
    InputEvent event;
    while(app->running) {
        if(furi_message_queue_get(app->event_queue, &event, 100) == FuriStatusOk) {
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                app->running = false;
            } else {
                sc_handle_input(app, &event);
            }
        }
        view_port_update(app->view_port);
    }

    // Cleanup
    app->worker_running = false;
    furi_thread_join(app->worker_thread);
    furi_thread_free(app->worker_thread);

    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);

    radio_device_loader_end(app->radio_device);
    subghz_devices_deinit();

    furi_record_close(RECORD_NOTIFICATION);
    furi_message_queue_free(app->event_queue);
    furi_mutex_free(app->mutex);
    free(app);

    return 0;
}
