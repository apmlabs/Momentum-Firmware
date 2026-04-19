#include "spectrum_check.h"
#include <stdlib.h>
#include <string.h>
#include <float_tools.h>
#include "protocols/sc_protocol_registry.h"

extern const SubGhzProtocolRegistry subghz_protocol_registry;

static const char* sc_sort_names[] = {"Count", "RSSI", "Freq", "Recent"};
static const char* sc_mod_names[] = {
    "AM650", "AM270", "FM2.4", "FM47.6",
    "TPMS-F", "TPMS-O", "TPMS-G", "OOK40k", "FSK40k"};

// Custom CC1101 register presets (from ProtoView)

// TPMS1: 20kBaud 2FSK, 28.56kHz deviation, 325kHz BW
static const uint8_t sc_preset_tpms1_fsk[] = {
    CC1101_IOCFG0, 0x0D, CC1101_FSCTRL1, 0x06,
    CC1101_PKTCTRL0, 0x32, CC1101_PKTCTRL1, 0x04,
    CC1101_MDMCFG0, 0x00, CC1101_MDMCFG1, 0x02,
    CC1101_MDMCFG2, 0x04, CC1101_MDMCFG3, 0x93,
    CC1101_MDMCFG4, 0x59, CC1101_DEVIATN, 0x41,
    CC1101_MCSM0, 0x18, CC1101_FOCCFG, 0x16,
    CC1101_AGCCTRL0, 0x91, CC1101_AGCCTRL1, 0x00, CC1101_AGCCTRL2, 0x07,
    CC1101_WORCTRL, 0xFB, CC1101_FREND0, 0x10, CC1101_FREND1, 0x56,
    0, 0, // end
    0xC0, 0, 0, 0, 0, 0, 0, 0 // PATABLE
};

// TPMS2: 10kBaud OOK, 650kHz BW
static const uint8_t sc_preset_tpms2_ook[] = {
    CC1101_IOCFG0, 0x0D, CC1101_FIFOTHR, 0x07,
    CC1101_PKTCTRL0, 0x32, CC1101_FSCTRL1, 0x06,
    CC1101_MDMCFG0, 0x00, CC1101_MDMCFG1, 0x00,
    CC1101_MDMCFG2, 0x30, CC1101_MDMCFG3, 0x93,
    CC1101_MDMCFG4, 0x18, CC1101_MCSM0, 0x18,
    CC1101_FOCCFG, 0x18,
    CC1101_AGCCTRL0, 0x91, CC1101_AGCCTRL1, 0x00, CC1101_AGCCTRL2, 0x07,
    CC1101_WORCTRL, 0xFB, CC1101_FREND0, 0x11, CC1101_FREND1, 0xB6,
    0, 0, // end
    0, 0xC0, 0, 0, 0, 0, 0, 0 // PATABLE (OOK)
};

// TPMS3: 20kBaud GFSK, 19kHz dev, 325kHz BW (Toyota)
static const uint8_t sc_preset_tpms3_gfsk[] = {
    CC1101_IOCFG0, 0x0D, CC1101_FSCTRL1, 0x06,
    CC1101_PKTCTRL0, 0x32, CC1101_PKTCTRL1, 0x04,
    CC1101_MDMCFG0, 0x00, CC1101_MDMCFG1, 0x02,
    CC1101_MDMCFG2, 0x10, CC1101_MDMCFG3, 0x93,
    CC1101_MDMCFG4, 0x59, CC1101_DEVIATN, 0x34,
    CC1101_MCSM0, 0x18, CC1101_FOCCFG, 0x16,
    CC1101_AGCCTRL0, 0x80, CC1101_AGCCTRL1, 0x58, CC1101_AGCCTRL2, 0x87,
    CC1101_WORCTRL, 0xFB, CC1101_FREND0, 0x10, CC1101_FREND1, 0x56,
    0, 0,
    0xC0, 0, 0, 0, 0, 0, 0, 0
};

// OOK 40kBaud (short pulses ~25µs)
static const uint8_t sc_preset_ook_40k[] = {
    CC1101_IOCFG0, 0x0D, CC1101_FIFOTHR, 0x07,
    CC1101_PKTCTRL0, 0x32, CC1101_FSCTRL1, 0x06,
    CC1101_MDMCFG0, 0x00, CC1101_MDMCFG1, 0x00,
    CC1101_MDMCFG2, 0x30, CC1101_MDMCFG3, 0x93,
    CC1101_MDMCFG4, 0x1A, CC1101_MCSM0, 0x18,
    CC1101_FOCCFG, 0x18,
    CC1101_AGCCTRL0, 0x91, CC1101_AGCCTRL1, 0x00, CC1101_AGCCTRL2, 0x07,
    CC1101_WORCTRL, 0xFB, CC1101_FREND0, 0x11, CC1101_FREND1, 0xB6,
    0, 0,
    0, 0xC0, 0, 0, 0, 0, 0, 0
};

// FSK 40kBaud, 28kHz dev, 270kHz BW
static const uint8_t sc_preset_fsk_40k[] = {
    CC1101_IOCFG0, 0x0D, CC1101_FSCTRL1, 0x06,
    CC1101_PKTCTRL0, 0x32, CC1101_PKTCTRL1, 0x04,
    CC1101_MDMCFG0, 0x00, CC1101_MDMCFG1, 0x02,
    CC1101_MDMCFG2, 0x04, CC1101_MDMCFG3, 0x93,
    CC1101_MDMCFG4, 0x6A, CC1101_DEVIATN, 0x41,
    CC1101_MCSM0, 0x18, CC1101_FOCCFG, 0x16,
    CC1101_AGCCTRL0, 0x91, CC1101_AGCCTRL1, 0x00, CC1101_AGCCTRL2, 0x07,
    CC1101_WORCTRL, 0xFB, CC1101_FREND0, 0x10, CC1101_FREND1, 0x56,
    0, 0,
    0xC0, 0, 0, 0, 0, 0, 0, 0
};

// Preset loading helper
static void sc_load_preset(const SubGhzDevice* dev, uint8_t mod) {
    switch(mod) {
    case SCModAM650: subghz_devices_load_preset(dev, FuriHalSubGhzPresetOok650Async, NULL); break;
    case SCModAM270: subghz_devices_load_preset(dev, FuriHalSubGhzPresetOok270Async, NULL); break;
    case SCModFM238: subghz_devices_load_preset(dev, FuriHalSubGhzPreset2FSKDev238Async, NULL); break;
    case SCModFM476: subghz_devices_load_preset(dev, FuriHalSubGhzPreset2FSKDev476Async, NULL); break;
    case SCModTPMS_FSK: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_preset_tpms1_fsk); break;
    case SCModTPMS_OOK: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_preset_tpms2_ook); break;
    case SCModTPMS_GFSK: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_preset_tpms3_gfsk); break;
    case SCModOOK_40k: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_preset_ook_40k); break;
    case SCModFSK_40k: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_preset_fsk_40k); break;
    default: subghz_devices_load_preset(dev, FuriHalSubGhzPresetOok650Async, NULL); break;
    }
}

// Preset name strings for .sub file saving (one per SCMod enum value)
static const char* sc_preset_names[] = {
    "FuriHalSubGhzPresetOok650Async",
    "FuriHalSubGhzPresetOok270Async",
    "FuriHalSubGhzPreset2FSKDev238Async",
    "FuriHalSubGhzPreset2FSKDev476Async",
    "FuriHalSubGhzPresetCustom", // TPMS_FSK
    "FuriHalSubGhzPresetCustom", // TPMS_OOK
    "FuriHalSubGhzPresetCustom", // TPMS_GFSK
    "FuriHalSubGhzPresetCustom", // OOK_40k
    "FuriHalSubGhzPresetCustom", // FSK_40k
};


// Spectrum scan frequencies (32 points across 3 bands)
static const uint32_t spec_freqs[] = {
    300000000, 307000000, 310000000, 315000000, 318000000, 330000000, 340000000, 348000000,
    390000000, 410000000, 418000000, 425000000, 430000000, 433920000, 438000000, 445000000, 450000000, 455000000, 460000000, 464000000,
    779000000, 800000000, 820000000, 840000000, 860000000, 868000000, 880000000, 900000000, 915000000, 920000000, 925000000, 928000000,
};
#define SPEC_FREQ_COUNT 32

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

            bool is_locked = false;
            furi_mutex_acquire(app->mutex, FuriWaitForever);
            is_locked = app->locked;
            furi_mutex_release(app->mutex);

            if(is_locked) {
                // LOCK MODE: listen on locked freq with current preset, cycle on silence
                uint32_t freq = app->locked_freq;
                uint8_t mod = app->locked_mod;

                subghz_devices_idle(app->radio_device);
                sc_load_preset(app->radio_device, mod);
                subghz_devices_set_frequency(app->radio_device, freq);

                // Check RSSI first
                subghz_devices_set_rx(app->radio_device);
                furi_delay_us(2000);
                float rssi = subghz_devices_get_rssi(app->radio_device);
                subghz_devices_idle(app->radio_device);

                furi_mutex_acquire(app->mutex, FuriWaitForever);
                app->detected_freq = freq;
                app->detected_rssi = rssi;
                furi_mutex_release(app->mutex);

                if(rssi > app->trigger) {
                    // Signal present! Capture it
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    app->signal_found = true;
                    uint8_t slot = app->signal_count < SC_MAX_SIGNALS ? app->signal_count : SC_MAX_SIGNALS - 1;
                    SCSignal* sig = &app->signals[slot];
                    memset(sig, 0, sizeof(SCSignal));
                    sig->frequency = freq;
                    sig->modulation = mod;
                    sc_cap_buf = sig->raw_data;
                    sc_cap_idx = 0;
                    furi_mutex_release(app->mutex);

                    sc_load_preset(app->radio_device, mod);
                    subghz_devices_set_frequency(app->radio_device, freq);
                    subghz_devices_start_async_rx(app->radio_device, sc_raw_callback, app);
                    furi_delay_ms(400);
                    subghz_devices_stop_async_rx(app->radio_device);

                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    sig->raw_count = sc_cap_idx;
                    if(sig->raw_count > 10) {
                        sc_analyze_signal(sig); sc_decode_signal(app, sig);
                        if(app->signal_count < SC_MAX_SIGNALS) {
                            app->signal_count++;
                        }
                        app->signal_selected = slot;
                        if(sig->protocol_decoded) {
                            // Decoded! Unlock automatically
                            app->locked = false;
                        }
                    }
                    furi_mutex_release(app->mutex);
                } else {
                    // No signal — advance to next preset
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    app->signal_found = false;
                    app->locked_mod = (app->locked_mod + 1) % SCModCount;
                    furi_mutex_release(app->mutex);
                }
                furi_delay_ms(100); // Brief pause between cycles
            } else {
                // FREE SCAN MODE: scan all frequencies, capture first hit
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

                        // Always capture on AM650 (most likely to get data)
                        bool already_have = false;
                        for(uint8_t s = 0; s < app->signal_count; s++) {
                            if(app->signals[s].frequency == fine_freq) { already_have = true; break; }
                        }
                        if(!already_have) {
                            furi_mutex_release(app->mutex);
                            subghz_devices_idle(app->radio_device);
                            subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);
                            subghz_devices_set_frequency(app->radio_device, fine_freq);

                            furi_mutex_acquire(app->mutex, FuriWaitForever);
                            uint8_t slot = app->signal_count < SC_MAX_SIGNALS ? app->signal_count : SC_MAX_SIGNALS - 1;
                            SCSignal* sig = &app->signals[slot];
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
                            if(sig->raw_count > 10) {
                                sc_analyze_signal(sig); sc_decode_signal(app, sig);
                                if(app->signal_count < SC_MAX_SIGNALS) {
                                    app->signal_count++;
                                }
                                app->signal_selected = slot;
                            }
                            furi_mutex_release(app->mutex);
                        } else {
                            furi_mutex_release(app->mutex);
                        }
                    } else {
                        app->signal_found = false;
                        furi_mutex_release(app->mutex);
                    }
                } else {
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    app->signal_found = false;
                    furi_mutex_release(app->mutex);
                }
            } // end of free scan else block

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
                sc_load_preset(app->radio_device, app->modulation);
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
    char buf[48];
    #define BAR_W 3
    #define SEP1_X (8 * (BAR_W + 1))
    #define SEP2_X (SEP1_X + 2 + 12 * (BAR_W + 1))
    #define GRAPH_TOP 10
    #define GRAPH_BOT 54

    // Find noise floor (minimum across all channels)
    uint8_t noise = 80;
    for(uint8_t i = 0; i < 32; i++) {
        if(app->channel_ss[i] < noise) noise = app->channel_ss[i];
    }

    for(uint8_t i = 0; i < 32; i++) {
        uint8_t x;
        if(i < 8) x = i * (BAR_W + 1);
        else if(i < 20) x = SEP1_X + 2 + (i - 8) * (BAR_W + 1);
        else x = SEP2_X + 2 + (i - 20) * (BAR_W + 1);

        // Subtract noise floor
        int8_t val = (int8_t)app->channel_ss[i] - (int8_t)noise;
        if(val < 0) val = 0;
        uint8_t h = val * (GRAPH_BOT - GRAPH_TOP) / 50;
        if(h > (GRAPH_BOT - GRAPH_TOP)) h = GRAPH_BOT - GRAPH_TOP;
        if(h > 0) {
            canvas_draw_box(canvas, x, GRAPH_BOT - h, BAR_W, h);
        }
        // Peak marker
        if(i == app->max_rssi_channel && app->max_rssi > -90.0f) {
            canvas_draw_frame(canvas, x - 1, GRAPH_BOT - h - 2, BAR_W + 2, h + 3);
        }
    }

    // Band separators
    for(uint8_t y = GRAPH_TOP; y < GRAPH_BOT; y += 2) {
        canvas_draw_dot(canvas, SEP1_X, y);
        canvas_draw_dot(canvas, SEP2_X, y);
    }

    // Decoded signal markers (triangles at bottom)
    for(uint8_t s = 0; s < app->signal_count; s++) {
        if(!app->signals[s].protocol_decoded) continue;
        uint32_t f = app->signals[s].frequency;
        // Find which channel this freq is closest to
        uint8_t closest = 0;
        uint32_t min_diff = UINT32_MAX;
        for(uint8_t i = 0; i < 32; i++) {
            uint32_t diff = (f > spec_freqs[i]) ? f - spec_freqs[i] : spec_freqs[i] - f;
            if(diff < min_diff) { min_diff = diff; closest = i; }
        }
        uint8_t x;
        if(closest < 8) x = closest * (BAR_W + 1) + 1;
        else if(closest < 20) x = SEP1_X + 2 + (closest - 8) * (BAR_W + 1) + 1;
        else x = SEP2_X + 2 + (closest - 20) * (BAR_W + 1) + 1;
        canvas_draw_dot(canvas, x, GRAPH_BOT + 2);
        canvas_draw_dot(canvas, x - 1, GRAPH_BOT + 3);
        canvas_draw_dot(canvas, x + 1, GRAPH_BOT + 3);
    }

    // Band labels
    canvas_draw_str(canvas, 2, 63, "300");
    canvas_draw_str(canvas, SEP1_X + 4, 63, "400");
    canvas_draw_str(canvas, SEP2_X + 4, 63, "900");

    // Top: peak info + protocol if known
    if(app->max_rssi > -90.0f) {
        uint32_t peak_freq = spec_freqs[app->max_rssi_channel];
        const char* proto = NULL;
        for(uint8_t s = 0; s < app->signal_count; s++) {
            if(app->signals[s].protocol_decoded && app->signals[s].frequency == peak_freq) {
                proto = app->signals[s].protocol_name;
                break;
            }
        }
        if(proto) {
            snprintf(buf, sizeof(buf), "%.0fdB %s", (double)app->max_rssi, proto);
        } else {
            snprintf(buf, sizeof(buf), "%.0fdB %ld.%02ld",
                (double)app->max_rssi,
                peak_freq / 1000000, (peak_freq / 10000) % 100);
        }
        canvas_draw_str(canvas, 0, 7, buf);
    }
}

static void sc_draw_freq_analyzer(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];
    canvas_set_font(canvas, FontPrimary);
    if(app->locked) {
        snprintf(buf, sizeof(buf), "LOCK %ld.%03ld [%s]",
            app->locked_freq / 1000000 % 1000, app->locked_freq / 1000 % 1000,
            sc_mod_names[app->locked_mod]);
        canvas_draw_box(canvas, 0, 0, 128, 11);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str(canvas, 2, 9, buf);
        canvas_set_color(canvas, ColorBlack);
    } else if(app->detected_freq > 0) {
        snprintf(buf, sizeof(buf), "%03ld.%03ld MHz  %.0fdB",
            app->detected_freq / 1000000 % 1000, app->detected_freq / 1000 % 1000,
            (double)app->detected_rssi);
        if(app->signal_found) { canvas_draw_box(canvas, 0, 0, 128, 11); canvas_set_color(canvas, ColorWhite); }
        canvas_draw_str(canvas, 2, 9, buf);
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_str(canvas, 2, 9, "Scanning...");
    }
    canvas_set_font(canvas, FontSecondary);
    snprintf(buf, sizeof(buf), "T:%.0f  %d sig  %s", (double)app->trigger, app->signal_count, app->log_size > 0 ? sc_sort_names[app->log_sort] : "");
    canvas_draw_str(canvas, 0, 20, buf);

    canvas_set_font(canvas, FontKeyboard);
    for(uint8_t i = 0; i < 5 && (app->log_scroll + i) < app->log_size; i++) {
        SCLogEntry* e = &app->log[app->log_scroll + i];
        snprintf(buf, sizeof(buf), "%03ld.%03ld %2dx %2ddB",
            e->frequency / 1000000 % 1000, e->frequency / 1000 % 1000,
            e->count, e->rssi_max);
        canvas_draw_str(canvas, 0, 28 + i * 8, buf);
    }
    // Page indicator
    if(app->log_size > 5) {
        uint8_t page = app->log_scroll / 5 + 1;
        uint8_t pages = (app->log_size + 4) / 5;
        snprintf(buf, sizeof(buf), "%d/%d", page, pages);
        canvas_draw_str(canvas, 105, 63, buf);
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

    // Line 1: signal index + freq + modulation (compact)
    snprintf(buf, sizeof(buf), "%d/%d  %ld.%03ld %s",
        app->signal_selected + 1, app->signal_count,
        sig->frequency / 1000000, (sig->frequency / 1000) % 1000,
        sc_mod_names[sig->modulation]);
    canvas_draw_str(canvas, 0, 7, buf);

    if(sig->protocol_decoded) {
        // Line 2: protocol name (bold)
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 0, 19, sig->protocol_name);
        canvas_set_font(canvas, FontSecondary);

        // Lines 3-6: decoded string (up to 4 lines)
        if(sig->decoded_string[0]) {
            const char* p = sig->decoded_string;
            uint8_t y = 29;
            for(uint8_t line = 0; line < 4 && *p && y <= 57; line++) {
                const char* nl = strchr(p, '\n');
                uint8_t len = nl ? (uint8_t)(nl - p) : strlen(p);
                if(len > 32) len = 32;
                char tmp[33];
                memcpy(tmp, p, len); tmp[len] = 0;
                canvas_draw_str(canvas, 0, y, tmp);
                y += 9;
                p += len;
                if(*p == '\n') p++;
            }
        } else {
            snprintf(buf, sizeof(buf), "Te:%ldus  %d pulses  %ldms",
                sig->min_pulse_us, sig->pulse_count, sig->total_duration_us / 1000);
            canvas_draw_str(canvas, 0, 29, buf);
        }
    } else if(sig->analyzed) {
        snprintf(buf, sizeof(buf), "%d pulses  Te:%ldus  %ldms",
            sig->pulse_count, sig->min_pulse_us < UINT32_MAX ? sig->min_pulse_us : 0,
            sig->total_duration_us / 1000);
        canvas_draw_str(canvas, 0, 19, buf);
        if(sig->est_rate_hz > 0) {
            snprintf(buf, sizeof(buf), "Rate: ~%ldHz", sig->est_rate_hz);
            canvas_draw_str(canvas, 0, 29, buf);
        }
    } else {
        canvas_draw_str(canvas, 0, 19, "Not analyzed");
    }
    // Bottom: controls
    canvas_draw_str(canvas, 0, 63, "\x12\x13:sig OK:cap Long:save");
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

    // Multi-row waveform: 4 rows, 12px each
    static const uint32_t scales[] = {50, 100, 200, 500, 1000, 2000};
    #define SCALE_COUNT 6
    uint8_t zoom = app->waveform_zoom < SCALE_COUNT ? app->waveform_zoom : 0;
    uint32_t us_per_pixel = scales[zoom];

    uint16_t idx = app->waveform_scroll;
    uint32_t remaining_dur = 0;
    bool level = false;

    for(uint8_t row = 0; row < 4 && idx < sig->raw_count; row++) {
        uint8_t y_base = 2 + row * 15;
        for(uint8_t x = 0; x < 128 && idx < sig->raw_count; x++) {
            if(remaining_dur < us_per_pixel / 2) {
                int32_t val = sig->raw_data[idx];
                level = val > 0;
                remaining_dur = level ? (uint32_t)val : (uint32_t)(-val);
                idx++;
            }
            uint8_t y = level ? y_base : y_base + 10;
            canvas_draw_dot(canvas, x, y);
            if(remaining_dur > us_per_pixel)
                remaining_dur -= us_per_pixel;
            else
                remaining_dur = 0;
        }
    }

    // Bottom info overlay
    canvas_set_font(canvas, FontSecondary);
    char buf[48];
    snprintf(buf, sizeof(buf), "%d/%d %ld.%03ld %s %ldus/px",
        app->signal_selected + 1, app->signal_count,
        sig->frequency / 1000000, (sig->frequency / 1000) % 1000,
        sig->protocol_decoded ? sig->protocol_name : sc_mod_names[sig->modulation],
        us_per_pixel);
    canvas_draw_str(canvas, 0, 63, buf);
}

static void sc_draw_callback(Canvas* canvas, void* ctx) {
    SpectrumCheckApp* app = ctx;
    furi_mutex_acquire(app->mutex, FuriWaitForever);
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    // No shared header — each view draws its own
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
            if(event->type == InputTypeLong || event->type == InputTypeRepeat) {
                app->trigger -= SC_TRIGGER_STEP;
                if(app->trigger < SC_RSSI_MIN) app->trigger = SC_RSSI_MIN;
            } else {
                // Page up in log
                if(app->log_scroll >= 5) app->log_scroll -= 5;
                else app->log_scroll = 0;
            }
        } else if(app->current_view == SCViewDecoder) {
            // Browse signals
            if(app->signal_selected > 0) app->signal_selected--;
        } else if(app->current_view == SCViewWaveform) {
            if(event->type == InputTypeLong || event->type == InputTypeRepeat) {
                if(app->waveform_scroll >= 10) app->waveform_scroll -= 10;
                else app->waveform_scroll = 0;
            } else {
                // Page back (jump ~50 samples)
                if(app->waveform_scroll >= 50) app->waveform_scroll -= 50;
                else app->waveform_scroll = 0;
            }
        }
        break;

    case InputKeyRight:
        if(app->current_view == SCViewFreqAnalyzer) {
            if(event->type == InputTypeLong || event->type == InputTypeRepeat) {
                app->trigger += SC_TRIGGER_STEP;
                if(app->trigger > SC_RSSI_MAX) app->trigger = SC_RSSI_MAX;
            } else {
                // Page down in log
                if(app->log_scroll + 5 < app->log_size) app->log_scroll += 5;
            }
        } else if(app->current_view == SCViewDecoder) {
            if(app->signal_selected + 1 < app->signal_count) app->signal_selected++;
        } else if(app->current_view == SCViewWaveform) {
            SCSignal* sig = app->signal_count > 0 ? &app->signals[app->signal_selected] : NULL;
            if(sig) {
                if(event->type == InputTypeLong || event->type == InputTypeRepeat) {
                    app->waveform_scroll += 10;
                } else {
                    app->waveform_scroll += 50; // Page forward
                }
                if(app->waveform_scroll >= sig->raw_count) app->waveform_scroll = sig->raw_count > 1 ? sig->raw_count - 1 : 0;
            }
        }
        break;

    case InputKeyOk:
        if(event->type == InputTypeLong) {
            // Long OK: save current signal, or cycle sort on freq analyzer
            if(app->current_view == SCViewFreqAnalyzer) {
                app->log_sort = (app->log_sort + 1) % SCLogSortModes;
                sc_log_sort(app->log, app->log_size, app->log_sort);
                app->log_scroll = 0;
            } else if(app->signal_count > 0 && (app->current_view == SCViewDecoder || app->current_view == SCViewWaveform)) {
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
                if(app->max_rssi > -90.0f && app->max_rssi_channel < 32) {
                    app->detected_freq = spec_freqs[app->max_rssi_channel];
                    app->frequency = app->detected_freq;
                }
            } else if(app->current_view == SCViewFreqAnalyzer) {
                if(app->locked) {
                    // Unlock
                    app->locked = false;
                } else if(app->log_size > 0) {
                    // Lock onto the top entry in current sort
                    SCLogEntry* e = &app->log[app->log_scroll];
                    app->locked = true;
                    app->locked_freq = e->frequency;
                    app->locked_mod = 0;
                } else {
                    // No log entries — cycle sort
                    app->log_sort = (app->log_sort + 1) % SCLogSortModes;
                    sc_log_sort(app->log, app->log_size, app->log_sort);
                    app->log_scroll = 0;
                }
            } else if(app->current_view == SCViewDecoder) {
                // Cycle modulation and trigger new capture
                app->modulation = (app->modulation + 1) % SCModCount;
                app->capturing = true;
                app->capture_done = false;
            } else if(app->current_view == SCViewWaveform) {
                app->waveform_zoom = (app->waveform_zoom + 1) % 6;
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
