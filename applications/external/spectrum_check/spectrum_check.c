#include "spectrum_check.h"
#include <stdlib.h>
#include <string.h>
#include <float_tools.h>

extern const SubGhzProtocolRegistry subghz_protocol_registry;

static const uint32_t sc_spacing[] = {196078, 39215, 784313};
static const uint32_t sc_step[] = {5000, 1000, 20000};
static const char* sc_width_names[] = {"Wide", "Narrow", "UWide"};
static const char* sc_view_names[] = {"SPECTRUM", "FREQ ANALYZER", "DECODER", "WAVEFORM"};
static const char* sc_sort_names[] = {"Count", "RSSI", "Freq", "Recent"};
static const char* sc_mod_names[] = {"AM650", "AM270", "FM2.4", "FM47.6"};
static const FuriHalSubGhzPreset sc_presets[] = {
    FuriHalSubGhzPresetOok650Async, FuriHalSubGhzPresetOok270Async,
    FuriHalSubGhzPreset2FSKDev238Async, FuriHalSubGhzPreset2FSKDev476Async,
};
static const char* sc_preset_names[] = {
    "FuriHalSubGhzPresetOok650Async", "FuriHalSubGhzPresetOok270Async",
    "FuriHalSubGhzPreset2FSKDev238Async", "FuriHalSubGhzPreset2FSKDev476Async",
};

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
            log[j + 1] = log[j]; j--;
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
        app->log[app->log_size] = (SCLogEntry){freq, 1, rssi_u8, app->log_seq++};
        app->log_size++;
        sc_log_sort(app->log, app->log_size, app->log_sort);
    }
}

// ============== Signal analysis ==============
static void sc_analyze_signal(SCSignal* sig) {
    if(sig->raw_count < 4) return;
    sig->pulse_count = sig->raw_count;
    sig->total_duration_us = 0;
    sig->min_pulse_us = UINT32_MAX;
    for(uint16_t i = 0; i < sig->raw_count; i++) {
        uint32_t d = sig->raw_data[i] > 0 ? (uint32_t)sig->raw_data[i] : (uint32_t)(-sig->raw_data[i]);
        sig->total_duration_us += d;
        if(d < sig->min_pulse_us && d > 50) sig->min_pulse_us = d;
    }
    if(sig->min_pulse_us < UINT32_MAX && sig->min_pulse_us > 0)
        sig->est_rate_hz = 1000000 / sig->min_pulse_us;
    else
        sig->est_rate_hz = 0;
    sig->analyzed = true;
}

// ============== Raw capture callback ==============
static volatile uint16_t sc_cap_idx;
static int32_t* sc_cap_buf;
static volatile bool sc_cap_last_level;

static void sc_raw_callback(bool level, uint32_t duration, void* context) {
    UNUSED(context);
    if(sc_cap_idx >= SC_RAW_PER_SIGNAL) return;
    sc_cap_buf[sc_cap_idx] = level ? (int32_t)duration : -(int32_t)duration;
    sc_cap_idx++;
    sc_cap_last_level = level;
}

// ============== Save signal to .sub ==============
static void sc_save_signal(SpectrumCheckApp* app, SCSignal* sig) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(storage, "/ext/subghz");
    storage_simply_mkdir(storage, "/ext/subghz/spectrum_check");

    FuriString* path = furi_string_alloc();
    // Generate filename from frequency
    furi_string_printf(path, "/ext/subghz/spectrum_check/%ld_%s.sub",
        sig->frequency / 1000, sc_mod_names[sig->modulation]);

    FlipperFormat* ff = flipper_format_file_alloc(storage);
    do {
        if(!flipper_format_file_open_always(ff, furi_string_get_cstr(path))) break;
        if(!flipper_format_write_header_cstr(ff, "Flipper SubGhz RAW File", 1)) break;
        if(!flipper_format_write_uint32(ff, "Frequency", &sig->frequency, 1)) break;
        if(!flipper_format_write_string_cstr(ff, "Preset", sc_preset_names[sig->modulation])) break;
        if(!flipper_format_write_string_cstr(ff, "Protocol", "RAW")) break;
        // Write raw data in chunks
        uint16_t written = 0;
        while(written < sig->raw_count) {
            uint16_t chunk = sig->raw_count - written;
            if(chunk > 128) chunk = 128;
            if(!flipper_format_write_int32(ff, "RAW_Data", &sig->raw_data[written], chunk)) break;
            written += chunk;
        }
    } while(0);
    flipper_format_free(ff);
    furi_string_free(path);
    furi_record_close(RECORD_STORAGE);

    notification_message(app->notifications, &sequence_success);
}

// ============== Worker thread ==============
static int32_t sc_worker(void* ctx) {
    SpectrumCheckApp* app = ctx;

    while(app->worker_running) {
        if(app->current_view == SCViewSpectrum) {
            uint32_t spacing = sc_spacing[app->width];
            uint32_t ch0 = app->frequency - (SC_NUM_CHANNELS / 2) * spacing;
            subghz_devices_idle(app->radio_device);
            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);

            float best_rssi = -200.0f;
            uint8_t best_ch = 0;
            for(uint8_t i = 0; i < SC_NUM_CHANNELS && app->worker_running; i++) {
                subghz_devices_set_frequency(app->radio_device, ch0 + i * spacing);
                subghz_devices_set_rx(app->radio_device);
                furi_delay_us(300);
                float rssi = subghz_devices_get_rssi(app->radio_device);
                subghz_devices_idle(app->radio_device);

                furi_mutex_acquire(app->mutex, FuriWaitForever);
                int8_t db = (int8_t)rssi + 138;
                app->channel_ss[i] = (db < 0) ? 0 : ((db > 80) ? 80 : (uint8_t)db);
                if(rssi > best_rssi) { best_rssi = rssi; best_ch = i; }
                furi_mutex_release(app->mutex);
            }
            furi_mutex_acquire(app->mutex, FuriWaitForever);
            app->max_rssi = best_rssi;
            app->max_rssi_channel = best_ch;
            furi_mutex_release(app->mutex);

        } else if(app->current_view == SCViewFreqAnalyzer) {
            static const uint32_t scan_freqs[] = {
                300000000, 303875000, 310000000, 315000000, 318000000,
                390000000, 418000000, 433075000, 433420000, 433920000,
                434420000, 434775000, 438900000,
                868350000, 868950000, 915000000, 925000000,
            };
            subghz_devices_idle(app->radio_device);
            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);

            float best_rssi = -127.0f;
            uint32_t best_freq = 0;
            for(uint8_t i = 0; i < 17 && app->worker_running; i++) {
                subghz_devices_set_frequency(app->radio_device, scan_freqs[i]);
                subghz_devices_set_rx(app->radio_device);
                furi_delay_us(2000);
                float rssi = subghz_devices_get_rssi(app->radio_device);
                subghz_devices_idle(app->radio_device);
                if(rssi > best_rssi) { best_rssi = rssi; best_freq = scan_freqs[i]; }
            }

            if(best_rssi > app->trigger && best_freq > 0) {
                // Fine scan
                subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok270Async, NULL);
                float fine_rssi = -127.0f;
                uint32_t fine_freq = 0;
                for(uint32_t f = best_freq - 300000; f <= best_freq + 300000; f += 20000) {
                    subghz_devices_set_frequency(app->radio_device, f);
                    subghz_devices_set_rx(app->radio_device);
                    furi_delay_us(2000);
                    float rssi = subghz_devices_get_rssi(app->radio_device);
                    subghz_devices_idle(app->radio_device);
                    if(rssi > fine_rssi) { fine_rssi = rssi; fine_freq = f; }
                }
                furi_mutex_acquire(app->mutex, FuriWaitForever);
                if(fine_rssi > app->trigger) {
                    app->signal_found = true;
                    app->detected_freq = fine_freq;
                    app->detected_rssi = fine_rssi;
                    sc_log_update(app, fine_freq, fine_rssi);

                    // Auto-capture into signal library
                    if(app->signal_count < SC_MAX_SIGNALS) {
                        bool already = false;
                        for(uint8_t s = 0; s < app->signal_count; s++) {
                            if(app->signals[s].frequency == fine_freq) { already = true; break; }
                        }
                        if(!already) {
                            // Quick capture
                            furi_mutex_release(app->mutex);
                            subghz_devices_idle(app->radio_device);
                            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
                            subghz_devices_set_frequency(app->radio_device, fine_freq);

                            furi_mutex_acquire(app->mutex, FuriWaitForever);
                            SCSignal* sig = &app->signals[app->signal_count];
                            memset(sig, 0, sizeof(SCSignal));
                            sig->frequency = fine_freq;
                            sig->modulation = SCModAM650;
                            sc_cap_buf = sig->raw_data;
                            sc_cap_idx = 0;
                            furi_mutex_release(app->mutex);

                            subghz_devices_start_async_rx(app->radio_device, sc_raw_callback, app);
                            furi_delay_ms(400);
                            subghz_devices_stop_async_rx(app->radio_device);

                            furi_mutex_acquire(app->mutex, FuriWaitForever);
                            sig->raw_count = sc_cap_idx;
                            sc_analyze_signal(sig);
                            if(sig->raw_count > 10) {
                                app->signal_count++;
                                app->signal_selected = app->signal_count - 1;
                            }
                        }
                    }
                } else {
                    app->signal_found = false;
                }
                furi_mutex_release(app->mutex);
            } else {
                furi_mutex_acquire(app->mutex, FuriWaitForever);
                app->signal_found = false;
                furi_mutex_release(app->mutex);
            }

        } else if(app->current_view == SCViewDecoder || app->current_view == SCViewWaveform) {
            // Only capture when explicitly requested
            if(app->capturing) {
                uint32_t freq = app->detected_freq ? app->detected_freq : app->frequency;
                subghz_devices_idle(app->radio_device);
                subghz_devices_load_preset(app->radio_device, sc_presets[app->modulation], NULL);
                subghz_devices_set_frequency(app->radio_device, freq);

                furi_mutex_acquire(app->mutex, FuriWaitForever);
                SCSignal* sig;
                if(app->signal_count < SC_MAX_SIGNALS) {
                    sig = &app->signals[app->signal_count];
                } else {
                    sig = &app->signals[SC_MAX_SIGNALS - 1]; // overwrite last
                }
                memset(sig, 0, sizeof(SCSignal));
                sig->frequency = freq;
                sig->modulation = app->modulation;
                sc_cap_buf = sig->raw_data;
                sc_cap_idx = 0;
                furi_mutex_release(app->mutex);

                subghz_devices_start_async_rx(app->radio_device, sc_raw_callback, app);
                furi_delay_ms(500);
                subghz_devices_stop_async_rx(app->radio_device);
                subghz_devices_idle(app->radio_device);

                furi_mutex_acquire(app->mutex, FuriWaitForever);
                sig->raw_count = sc_cap_idx;
                sc_analyze_signal(sig);
                if(sig->raw_count > 4 && app->signal_count < SC_MAX_SIGNALS) {
                    app->signal_count++;
                }
                app->signal_selected = app->signal_count > 0 ? app->signal_count - 1 : 0;
                app->capturing = false;
                app->capture_done = true;
                furi_mutex_release(app->mutex);
            } else {
                furi_delay_ms(100);
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
    for(uint8_t i = 0; i < SC_NUM_CHANNELS; i++) {
        uint8_t h = app->channel_ss[i] * 40 / 80;
        if(h > 0) canvas_draw_line(canvas, SC_FREQ_START_X + i, SC_FREQ_BOTTOM_Y, SC_FREQ_START_X + i, SC_FREQ_BOTTOM_Y - h);
    }
    if(app->max_rssi > -90.0f) {
        uint8_t px = SC_FREQ_START_X + app->max_rssi_channel;
        canvas_draw_line(canvas, px, 10, px, SC_FREQ_BOTTOM_Y);
        snprintf(buf, sizeof(buf), "%.0fdB", (double)app->max_rssi);
        canvas_draw_str(canvas, px > 80 ? px - 30 : px + 2, 18, buf);
    }
    uint32_t spacing = sc_spacing[app->width];
    uint32_t left = app->frequency - (SC_NUM_CHANNELS / 2) * spacing;
    uint32_t right = app->frequency + (SC_NUM_CHANNELS / 2) * spacing;
    snprintf(buf, sizeof(buf), "%ld.%01ld", left / 1000000, (left / 100000) % 10);
    canvas_draw_str(canvas, 0, 62, buf);
    snprintf(buf, sizeof(buf), "%ld.%01ld", right / 1000000, (right / 100000) % 10);
    canvas_draw_str(canvas, 88, 62, buf);
    canvas_draw_str(canvas, 48, 62, sc_width_names[app->width]);
}

static void sc_draw_freq_analyzer(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];
    canvas_set_font(canvas, FontBigNumbers);
    if(app->detected_freq > 0) {
        snprintf(buf, sizeof(buf), "%03ld.%03ld", app->detected_freq / 1000000 % 1000, app->detected_freq / 1000 % 1000);
        if(app->signal_found) { canvas_draw_box(canvas, 2, 10, 124, 19); canvas_set_color(canvas, ColorWhite); }
        canvas_draw_str(canvas, 6, 26, buf);
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_str(canvas, 6, 26, "----.---");
    }
    canvas_set_font(canvas, FontSecondary);
    snprintf(buf, sizeof(buf), "T:%.0f Sig:%d", (double)app->trigger, app->signal_count);
    canvas_draw_str(canvas, 0, 36, buf);

    canvas_set_font(canvas, FontKeyboard);
    for(uint8_t i = 0; i < 3 && (app->log_scroll + i) < app->log_size; i++) {
        SCLogEntry* e = &app->log[app->log_scroll + i];
        snprintf(buf, sizeof(buf), "%03ld.%03ld x%d", e->frequency / 1000000 % 1000, e->frequency / 1000 % 1000, e->count);
        canvas_draw_str(canvas, 0, 44 + i * 9, buf);
        uint8_t bar = e->rssi_max > 40 ? 40 : e->rssi_max;
        for(uint8_t b = 0; b < bar; b++) { if(b % 4) canvas_draw_dot(canvas, 85 + b, 41 + i * 9); }
    }
    canvas_set_font(canvas, FontSecondary);
    if(app->log_size > 0) canvas_draw_str(canvas, 85, 36, sc_sort_names[app->log_sort]);
}

static void sc_draw_decoder(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];
    canvas_set_font(canvas, FontSecondary);

    if(app->signal_count == 0) {
        canvas_draw_str(canvas, 4, 20, "No signals captured yet.");
        canvas_draw_str(canvas, 4, 32, "Use Freq Analyzer to find");
        canvas_draw_str(canvas, 4, 42, "signals, or press OK to");
        canvas_draw_str(canvas, 4, 52, "capture on current freq.");
        return;
    }

    SCSignal* sig = &app->signals[app->signal_selected];
    // Header: signal index + freq
    snprintf(buf, sizeof(buf), "[%d/%d] %ld.%03ld %s",
        app->signal_selected + 1, app->signal_count,
        sig->frequency / 1000000, (sig->frequency / 1000) % 1000,
        sc_mod_names[sig->modulation]);
    canvas_draw_str(canvas, 0, 18, buf);

    if(sig->analyzed) {
        snprintf(buf, sizeof(buf), "Pulses: %d", sig->pulse_count);
        canvas_draw_str(canvas, 0, 30, buf);
        snprintf(buf, sizeof(buf), "Duration: %ldms", sig->total_duration_us / 1000);
        canvas_draw_str(canvas, 0, 40, buf);
        if(sig->min_pulse_us < UINT32_MAX) {
            snprintf(buf, sizeof(buf), "Te: %ldus  Rate: %ldHz", sig->min_pulse_us, sig->est_rate_hz);
            canvas_draw_str(canvas, 0, 50, buf);
        }
    } else {
        canvas_draw_str(canvas, 0, 30, "Not analyzed");
    }
    // Bottom hints
    canvas_draw_str(canvas, 0, 62, "L/R:sig OK:cap Long:save");
}

static void sc_draw_waveform(Canvas* canvas, SpectrumCheckApp* app) {
    if(app->signal_count == 0) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 10, 32, "No signal captured yet");
        return;
    }
    SCSignal* sig = &app->signals[app->signal_selected];
    if(sig->raw_count == 0) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 10, 32, "Empty signal");
        return;
    }

    uint8_t zoom = app->waveform_zoom ? app->waveform_zoom : 1;
    uint16_t x = 0;
    uint16_t idx = app->waveform_scroll;
    while(x < 128 && idx < sig->raw_count) {
        int32_t val = sig->raw_data[idx];
        bool level = val > 0;
        uint32_t dur = level ? (uint32_t)val : (uint32_t)(-val);
        uint8_t pixels = dur / (100 / zoom);
        if(pixels == 0) pixels = 1;
        if(pixels > 128 - x) pixels = 128 - x;

        uint8_t y = level ? 15 : 45;
        canvas_draw_line(canvas, x, y, x + pixels, y);
        if(idx > app->waveform_scroll) {
            int32_t prev = sig->raw_data[idx - 1];
            uint8_t prev_y = prev > 0 ? 15 : 45;
            if(prev_y != y) canvas_draw_line(canvas, x, 15, x, 45);
        }
        x += pixels;
        idx++;
    }
    canvas_set_font(canvas, FontSecondary);
    char buf[40];
    snprintf(buf, sizeof(buf), "[%d/%d] %d/%d z:%d",
        app->signal_selected + 1, app->signal_count,
        app->waveform_scroll, sig->raw_count, zoom);
    canvas_draw_str(canvas, 0, 62, buf);
}

static void sc_draw_callback(Canvas* canvas, void* ctx) {
    SpectrumCheckApp* app = ctx;
    furi_mutex_acquire(app->mutex, FuriWaitForever);
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    // Header
    char buf[32];
    canvas_draw_str(canvas, 0, 7, sc_view_names[app->current_view]);
    snprintf(buf, sizeof(buf), "%ld.%02ld %s",
        app->frequency / 1000000, (app->frequency / 10000) % 100,
        sc_mod_names[app->modulation]);
    canvas_draw_str(canvas, 70, 7, buf);

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
    furi_message_queue_put(((SpectrumCheckApp*)ctx)->event_queue, event, FuriWaitForever);
}

static void sc_handle_input(SpectrumCheckApp* app, InputEvent* event) {
    if(event->type != InputTypeShort && event->type != InputTypeRepeat && event->type != InputTypeLong) return;
    furi_mutex_acquire(app->mutex, FuriWaitForever);

    switch(event->key) {
    case InputKeyUp:
        app->current_view = app->current_view == 0 ? SCViewCount - 1 : app->current_view - 1;
        app->waveform_scroll = 0;
        break;
    case InputKeyDown:
        app->current_view = (app->current_view + 1) % SCViewCount;
        app->waveform_scroll = 0;
        break;

    case InputKeyLeft:
        if(app->current_view == SCViewSpectrum) {
            app->frequency -= sc_step[app->width] * 1000;
            if(app->frequency < 300000000) app->frequency = 300000000;
        } else if(app->current_view == SCViewFreqAnalyzer) {
            app->trigger -= SC_TRIGGER_STEP;
            if(app->trigger < SC_RSSI_MIN) app->trigger = SC_RSSI_MIN;
        } else if(app->current_view == SCViewDecoder) {
            // Browse signals
            if(app->signal_selected > 0) app->signal_selected--;
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
        } else if(app->current_view == SCViewDecoder) {
            if(app->signal_selected + 1 < app->signal_count) app->signal_selected++;
        } else if(app->current_view == SCViewWaveform) {
            SCSignal* sig = app->signal_count > 0 ? &app->signals[app->signal_selected] : NULL;
            if(sig && app->waveform_scroll + 1 < sig->raw_count) app->waveform_scroll++;
        }
        break;

    case InputKeyOk:
        if(event->type == InputTypeLong) {
            // Long OK: save current signal
            if(app->signal_count > 0 && (app->current_view == SCViewDecoder || app->current_view == SCViewWaveform)) {
                SCSignal* sig = &app->signals[app->signal_selected];
                if(sig->raw_count > 0) {
                    furi_mutex_release(app->mutex);
                    sc_save_signal(app, sig);
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                }
            }
        } else {
            // Short OK
            if(app->current_view == SCViewSpectrum) {
                // Lock peak frequency + cycle width
                if(app->max_rssi > -90.0f) {
                    uint32_t spacing = sc_spacing[app->width];
                    uint32_t peak = app->frequency - (SC_NUM_CHANNELS / 2) * spacing + app->max_rssi_channel * spacing;
                    app->detected_freq = peak;
                    app->frequency = peak;
                }
                app->width = (app->width + 1) % 3;
            } else if(app->current_view == SCViewFreqAnalyzer) {
                app->log_sort = (app->log_sort + 1) % SCLogSortModes;
                sc_log_sort(app->log, app->log_size, app->log_sort);
                app->log_scroll = 0;
            } else if(app->current_view == SCViewDecoder) {
                // Cycle modulation and trigger new capture
                app->modulation = (app->modulation + 1) % SCModCount;
                app->capturing = true;
                app->capture_done = false;
            } else if(app->current_view == SCViewWaveform) {
                app->waveform_zoom = app->waveform_zoom >= 4 ? 1 : app->waveform_zoom + 1;
            }
        }
        break;
    default: break;
    }
    furi_mutex_release(app->mutex);
}

// ============== App lifecycle ==============
int32_t spectrum_check_app(void* p) {
    UNUSED(p);
    SpectrumCheckApp* app = malloc(sizeof(SpectrumCheckApp));
    memset(app, 0, sizeof(SpectrumCheckApp));

    app->frequency = 433920000;
    app->trigger = -85.0f;
    app->width = SCWidthWide;
    app->waveform_zoom = 1;
    app->running = true;

    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    subghz_devices_init();
    app->radio_device = radio_device_loader_set(NULL, SubGhzRadioDeviceTypeInternal);

    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, sc_draw_callback, app);
    view_port_input_callback_set(app->view_port, sc_input_callback, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    app->worker_running = true;
    app->worker_thread = furi_thread_alloc_ex("SCWorker", 4096, sc_worker, app);
    furi_thread_start(app->worker_thread);

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
