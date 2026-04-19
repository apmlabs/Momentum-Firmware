#include "spectrum_check.h"
#include <stdlib.h>
#include <string.h>
#include <float_tools.h>
#include "protocols/sc_protocol_registry.h"

extern const SubGhzProtocolRegistry subghz_protocol_registry;

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

// Safe CC1101 bands (datasheet limits, no extended range)
typedef struct { uint32_t min; uint32_t center; uint32_t max; } SCBand;
static const SCBand sc_bands[] = {
    {300000000, 315000000, 348000000},  // 300 MHz band
    {387000000, 433920000, 464000000},  // 400 MHz band
    {779000000, 868000000, 928000000},  // 900 MHz band
};
#define SC_BAND_COUNT 3

static bool sc_freq_valid(uint32_t f) {
    for(uint8_t i = 0; i < SC_BAND_COUNT; i++) {
        if(f >= sc_bands[i].min && f <= sc_bands[i].max) return true;
    }
    return false;
}

// Full scan frequency list (all 3 bands, same as firmware)
static const uint32_t sc_scan_freqs[] = {
    300000000, 303875000, 310000000, 315000000, 318000000, 330000000, 345000000,
    390000000, 418000000, 430000000, 433075000, 433420000, 433920000,
    434420000, 434775000, 438900000,
    779000000, 868350000, 868950000, 906400000, 915000000, 925000000,
};
#define SC_SCAN_COUNT (sizeof(sc_scan_freqs) / sizeof(sc_scan_freqs[0]))

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

// ============== Protocol decoding (Layer 1: main registry) ==============
static volatile bool sc_decode_found;
static char sc_decode_name[32];
static char sc_decode_string[128];

static void sc_decode_callback(
    SubGhzReceiver* receiver,
    SubGhzProtocolDecoderBase* decoder_base,
    void* context) {
    UNUSED(receiver);
    UNUSED(context);
    if(sc_decode_found) return; // only take first match
    sc_decode_found = true;
    strncpy(sc_decode_name, decoder_base->protocol->name, sizeof(sc_decode_name) - 1);
    FuriString* text = furi_string_alloc();
    subghz_protocol_decoder_base_get_string(decoder_base, text);
    strncpy(sc_decode_string, furi_string_get_cstr(text), sizeof(sc_decode_string) - 1);
    furi_string_free(text);
}

static void sc_decode_signal(SpectrumCheckApp* app, SCSignal* sig) {
    if(sig->raw_count < 4 || !app->receiver) return;

    // Layer 1: Main protocol registry (50+ protocols)
    sc_decode_found = false;
    sc_decode_name[0] = 0;
    sc_decode_string[0] = 0;
    subghz_receiver_reset(app->receiver);
    for(uint16_t i = 0; i < sig->raw_count; i++) {
        bool level = sig->raw_data[i] > 0;
        uint32_t duration = level ? (uint32_t)sig->raw_data[i] : (uint32_t)(-sig->raw_data[i]);
        subghz_receiver_decode(app->receiver, level, duration);
        if(sc_decode_found) break;
    }

    // Layer 3: Extra protocols (weather, TPMS, POCSAG) if Layer 1 didn't match
    if(!sc_decode_found && app->extra_receiver) {
        subghz_receiver_reset(app->extra_receiver);
        for(uint16_t i = 0; i < sig->raw_count; i++) {
            bool level = sig->raw_data[i] > 0;
            uint32_t duration = level ? (uint32_t)sig->raw_data[i] : (uint32_t)(-sig->raw_data[i]);
            subghz_receiver_decode(app->extra_receiver, level, duration);
            if(sc_decode_found) break;
        }
    }

    if(sc_decode_found) {
        strncpy(sig->protocol_name, sc_decode_name, sizeof(sig->protocol_name) - 1);
        strncpy(sig->decoded_string, sc_decode_string, sizeof(sig->decoded_string) - 1);
        sig->protocol_decoded = true;
    } else {
        // Layer 2: timing heuristics
        if(sig->min_pulse_us >= 400 && sig->min_pulse_us <= 600 && sig->pulse_count > 60) {
            strncpy(sig->protocol_name, "Likely: Weather sensor", sizeof(sig->protocol_name) - 1);
            sig->protocol_decoded = true;
        } else if(sig->min_pulse_us >= 80 && sig->min_pulse_us <= 150) {
            strncpy(sig->protocol_name, "Likely: TPMS/FSK", sizeof(sig->protocol_name) - 1);
            sig->protocol_decoded = true;
        } else if(sig->min_pulse_us >= 200 && sig->min_pulse_us <= 350 && sig->pulse_count < 80) {
            strncpy(sig->protocol_name, "Likely: Remote/OOK", sizeof(sig->protocol_name) - 1);
            sig->protocol_decoded = true;
        }
    }
}

// ============== Raw capture callback ==============
static volatile uint16_t sc_cap_idx;
static int32_t* sc_cap_buf;

static void sc_raw_callback(bool level, uint32_t duration, void* context) {
    UNUSED(context);
    if(sc_cap_idx >= SC_RAW_PER_SIGNAL) return;
    sc_cap_buf[sc_cap_idx] = level ? (int32_t)duration : -(int32_t)duration;
    sc_cap_idx++;
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
            // Scan all 3 bands using fixed frequency points
            // Band 1: 300-348 (8 points), Band 2: 387-464 (12 points), Band 3: 779-928 (12 points)
            static const uint32_t spec_freqs[] = {
                // Band 1: 300 MHz
                300000000, 307000000, 310000000, 315000000, 318000000, 330000000, 340000000, 348000000,
                // Band 2: 400 MHz
                390000000, 410000000, 418000000, 425000000, 430000000, 433920000, 438000000, 445000000, 450000000, 455000000, 460000000, 464000000,
                // Band 3: 900 MHz
                779000000, 800000000, 820000000, 840000000, 860000000, 868000000, 880000000, 900000000, 915000000, 920000000, 925000000, 928000000,
            };
            #define SPEC_FREQ_COUNT 32
            #define SPEC_BAND1_END 8
            #define SPEC_BAND2_END 20

            subghz_devices_idle(app->radio_device);
            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);

            float best_rssi = -200.0f;
            uint8_t best_ch = 0;
            for(uint8_t i = 0; i < SPEC_FREQ_COUNT && app->worker_running; i++) {
                subghz_devices_set_frequency(app->radio_device, spec_freqs[i]);
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
            subghz_devices_idle(app->radio_device);
            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);

            float best_rssi = -127.0f;
            uint32_t best_freq = 0;
            for(uint8_t i = 0; i < SC_SCAN_COUNT && app->worker_running; i++) {
                subghz_devices_set_frequency(app->radio_device, sc_scan_freqs[i]);
                subghz_devices_set_rx(app->radio_device);
                furi_delay_us(2000);
                float rssi = subghz_devices_get_rssi(app->radio_device);
                subghz_devices_idle(app->radio_device);
                if(rssi > best_rssi) { best_rssi = rssi; best_freq = sc_scan_freqs[i]; }
            }

            if(best_rssi > app->trigger && best_freq > 0) {
                // Fine scan
                subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok270Async, NULL);
                float fine_rssi = -127.0f;
                uint32_t fine_freq = 0;
                for(uint32_t f = best_freq - 300000; f <= best_freq + 300000; f += 20000) {
                    if(!sc_freq_valid(f)) continue;
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

                    // Auto-capture: try all modulations, only keep if decoded
                    bool already_decoded = false;
                    for(uint8_t s = 0; s < app->signal_count; s++) {
                        if(app->signals[s].frequency == fine_freq && app->signals[s].protocol_decoded) {
                            already_decoded = true;
                            break;
                        }
                    }
                    if(!already_decoded) {
                        furi_mutex_release(app->mutex);
                        // Try each modulation preset
                        for(uint8_t mod = 0; mod < SCModCount && app->worker_running; mod++) {
                            subghz_devices_idle(app->radio_device);
                            subghz_devices_load_preset(app->radio_device, sc_presets[mod], NULL);
                            subghz_devices_set_frequency(app->radio_device, fine_freq);

                            furi_mutex_acquire(app->mutex, FuriWaitForever);
                            // Use a temp slot
                            uint8_t slot = app->signal_count < SC_MAX_SIGNALS ? app->signal_count : SC_MAX_SIGNALS - 1;
                            SCSignal* sig = &app->signals[slot];
                            memset(sig, 0, sizeof(SCSignal));
                            sig->frequency = fine_freq;
                            sig->modulation = mod;
                            sc_cap_buf = sig->raw_data;
                            sc_cap_idx = 0;
                            furi_mutex_release(app->mutex);

                            subghz_devices_start_async_rx(app->radio_device, sc_raw_callback, app);
                            furi_delay_ms(400);
                            subghz_devices_stop_async_rx(app->radio_device);

                            furi_mutex_acquire(app->mutex, FuriWaitForever);
                            sig->raw_count = sc_cap_idx;
                            if(sig->raw_count > 10) {
                                sc_analyze_signal(sig); sc_decode_signal(app, sig);
                                if(sig->protocol_decoded) {
                                    // Keep it!
                                    if(app->signal_count < SC_MAX_SIGNALS) {
                                        app->signal_count++;
                                    }
                                    app->signal_selected = slot;
                                    furi_mutex_release(app->mutex);
                                    break; // Found a match, stop trying modulations
                                }
                            }
                            furi_mutex_release(app->mutex);
                        }
                        furi_mutex_acquire(app->mutex, FuriWaitForever);
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
            if(!app->signal_found && app->detected_freq == 0) {
                furi_delay_ms(100);
                continue;
            }
            if(app->capturing) {
                uint32_t freq = app->detected_freq ? app->detected_freq : app->frequency;
                if(!sc_freq_valid(freq)) { app->capturing = false; furi_delay_ms(100); continue; }
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
                sc_analyze_signal(sig); sc_decode_signal(app, sig);
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
    // 32 channels across 128px = 3px per bar + 1px gap, plus 2 separator lines
    // Band 1: channels 0-7 (8 bars), Band 2: 8-19 (12 bars), Band 3: 20-31 (12 bars)
    #define BAR_W 3
    #define SEP1_X (8 * (BAR_W + 1))   // after band 1
    #define SEP2_X (SEP1_X + 2 + 12 * (BAR_W + 1)) // after band 2

    for(uint8_t i = 0; i < 32; i++) {
        uint8_t x;
        if(i < 8) {
            x = i * (BAR_W + 1);
        } else if(i < 20) {
            x = SEP1_X + 2 + (i - 8) * (BAR_W + 1);
        } else {
            x = SEP2_X + 2 + (i - 20) * (BAR_W + 1);
        }
        uint8_t h = app->channel_ss[i] * 38 / 80;
        if(h > 0) {
            canvas_draw_box(canvas, x, SC_FREQ_BOTTOM_Y - h, BAR_W, h);
        }
        // Peak marker
        if(i == app->max_rssi_channel && app->max_rssi > -90.0f) {
            canvas_draw_frame(canvas, x - 1, SC_FREQ_BOTTOM_Y - h - 2, BAR_W + 2, h + 3);
        }
    }

    // Band separator lines (dotted)
    for(uint8_t y = 12; y < SC_FREQ_BOTTOM_Y; y += 2) {
        canvas_draw_dot(canvas, SEP1_X, y);
        canvas_draw_dot(canvas, SEP2_X, y);
    }

    // Band labels
    canvas_draw_str(canvas, 2, 62, "300");
    canvas_draw_str(canvas, SEP1_X + 4, 62, "400");
    canvas_draw_str(canvas, SEP2_X + 4, 62, "900");

    // Peak info
    if(app->max_rssi > -90.0f) {
        snprintf(buf, sizeof(buf), "%.0fdB", (double)app->max_rssi);
        canvas_draw_str(canvas, 95, 62, buf);
    }
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
    snprintf(buf, sizeof(buf), "T:%.0f  %d sig  %s", (double)app->trigger, app->signal_count, app->log_size > 0 ? sc_sort_names[app->log_sort] : "");
    canvas_draw_str(canvas, 0, 36, buf);

    canvas_set_font(canvas, FontKeyboard);
    for(uint8_t i = 0; i < 3 && (app->log_scroll + i) < app->log_size; i++) {
        SCLogEntry* e = &app->log[app->log_scroll + i];
        snprintf(buf, sizeof(buf), "%03ld.%03ld %2dx %2ddB",
            e->frequency / 1000000 % 1000, e->frequency / 1000 % 1000,
            e->count, e->rssi_max);
        canvas_draw_str(canvas, 0, 44 + i * 9, buf);
    }
    canvas_set_font(canvas, FontSecondary);
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
        if(sig->protocol_decoded) {
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 0, 30, sig->protocol_name);
            canvas_set_font(canvas, FontSecondary);
            if(sig->decoded_string[0]) {
                // Draw first 2 lines of decoded string
                const char* p = sig->decoded_string;
                uint8_t y = 40;
                for(uint8_t line = 0; line < 2 && *p && y < 58; line++) {
                    const char* nl = strchr(p, '\n');
                    uint8_t len = nl ? (uint8_t)(nl - p) : strlen(p);
                    if(len > 30) len = 30;
                    char tmp[32];
                    memcpy(tmp, p, len); tmp[len] = 0;
                    canvas_draw_str(canvas, 0, y, tmp);
                    y += 10;
                    p += len;
                    if(*p == '\n') p++;
                }
            } else {
                snprintf(buf, sizeof(buf), "Te:%ldus Pulses:%d", sig->min_pulse_us, sig->pulse_count);
                canvas_draw_str(canvas, 0, 40, buf);
            }
        } else {
            snprintf(buf, sizeof(buf), "Pulses: %d", sig->pulse_count);
            canvas_draw_str(canvas, 0, 30, buf);
            snprintf(buf, sizeof(buf), "Duration: %ldms Te:%ldus", sig->total_duration_us / 1000, sig->min_pulse_us < UINT32_MAX ? sig->min_pulse_us : 0);
            canvas_draw_str(canvas, 0, 40, buf);
            if(sig->est_rate_hz > 0) {
                snprintf(buf, sizeof(buf), "Rate: ~%ldHz", sig->est_rate_hz);
                canvas_draw_str(canvas, 0, 50, buf);
            }
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
    if(app->current_view != SCViewFreqAnalyzer) {
        canvas_draw_str(canvas, 0, 7, sc_view_names[app->current_view]);
    }
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
        if(app->current_view == SCViewFreqAnalyzer) {
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
        if(app->current_view == SCViewFreqAnalyzer) {
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
                // Lock peak frequency for decoder/waveform
                static const uint32_t spec_freqs[] = {
                    300000000, 307000000, 310000000, 315000000, 318000000, 330000000, 340000000, 348000000,
                    390000000, 410000000, 418000000, 425000000, 430000000, 433920000, 438000000, 445000000, 450000000, 455000000, 460000000, 464000000,
                    779000000, 800000000, 820000000, 840000000, 860000000, 868000000, 880000000, 900000000, 915000000, 920000000, 925000000, 928000000,
                };
                if(app->max_rssi > -90.0f && app->max_rssi_channel < 32) {
                    app->detected_freq = spec_freqs[app->max_rssi_channel];
                    app->frequency = app->detected_freq;
                }
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

    // Protocol decoder setup (Layer 1)
    app->environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(app->environment, (void*)&subghz_protocol_registry);
    app->receiver = subghz_receiver_alloc_init(app->environment);
    subghz_receiver_set_filter(app->receiver, SubGhzProtocolFlag_Decodable);
    subghz_receiver_set_rx_callback(app->receiver, sc_decode_callback, app);

    // Layer 3: extra protocols (weather, TPMS, POCSAG)
    app->extra_environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(app->extra_environment, (void*)&sc_extra_protocol_registry);
    app->extra_receiver = subghz_receiver_alloc_init(app->extra_environment);
    subghz_receiver_set_filter(app->extra_receiver, SubGhzProtocolFlag_Decodable);
    subghz_receiver_set_rx_callback(app->extra_receiver, sc_decode_callback, app);

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
    subghz_receiver_free(app->receiver);
    subghz_environment_free(app->environment);
    subghz_receiver_free(app->extra_receiver);
    subghz_environment_free(app->extra_environment);
    furi_record_close(RECORD_NOTIFICATION);
    furi_message_queue_free(app->event_queue);
    furi_mutex_free(app->mutex);
    free(app);
    return 0;
}
