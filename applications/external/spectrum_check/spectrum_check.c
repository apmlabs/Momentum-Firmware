#include "spectrum_check.h"
#include <stdlib.h>
#include <string.h>
#include <float_tools.h>
#include "protocols/sc_protocol_registry.h"

extern const SubGhzProtocolRegistry subghz_protocol_registry;

static const char* sc_mod_names[] = {"AM650","AM270","FM2.4","FM47.6","TPMS-F","TPMS-O","TPMS-G","OOK40k","FSK40k"};
static const char* sc_sort_names[] = {"Count","RSSI","Freq","Recent"};
static const char* sc_preset_fnames[] = {
    "FuriHalSubGhzPresetOok650Async","FuriHalSubGhzPresetOok270Async",
    "FuriHalSubGhzPreset2FSKDev238Async","FuriHalSubGhzPreset2FSKDev476Async",
    "FuriHalSubGhzPresetCustom","FuriHalSubGhzPresetCustom",
    "FuriHalSubGhzPresetCustom","FuriHalSubGhzPresetCustom","FuriHalSubGhzPresetCustom"};

// Custom CC1101 presets
static const uint8_t sc_p_tpms1[]={CC1101_IOCFG0,0x0D,CC1101_FSCTRL1,0x06,CC1101_PKTCTRL0,0x32,CC1101_PKTCTRL1,0x04,CC1101_MDMCFG0,0x00,CC1101_MDMCFG1,0x02,CC1101_MDMCFG2,0x04,CC1101_MDMCFG3,0x93,CC1101_MDMCFG4,0x59,CC1101_DEVIATN,0x41,CC1101_MCSM0,0x18,CC1101_FOCCFG,0x16,CC1101_AGCCTRL0,0x91,CC1101_AGCCTRL1,0x00,CC1101_AGCCTRL2,0x07,CC1101_WORCTRL,0xFB,CC1101_FREND0,0x10,CC1101_FREND1,0x56,0,0,0xC0,0,0,0,0,0,0,0};
static const uint8_t sc_p_tpms2[]={CC1101_IOCFG0,0x0D,CC1101_FIFOTHR,0x07,CC1101_PKTCTRL0,0x32,CC1101_FSCTRL1,0x06,CC1101_MDMCFG0,0x00,CC1101_MDMCFG1,0x00,CC1101_MDMCFG2,0x30,CC1101_MDMCFG3,0x93,CC1101_MDMCFG4,0x18,CC1101_MCSM0,0x18,CC1101_FOCCFG,0x18,CC1101_AGCCTRL0,0x91,CC1101_AGCCTRL1,0x00,CC1101_AGCCTRL2,0x07,CC1101_WORCTRL,0xFB,CC1101_FREND0,0x11,CC1101_FREND1,0xB6,0,0,0,0xC0,0,0,0,0,0,0};
static const uint8_t sc_p_tpms3[]={CC1101_IOCFG0,0x0D,CC1101_FSCTRL1,0x06,CC1101_PKTCTRL0,0x32,CC1101_PKTCTRL1,0x04,CC1101_MDMCFG0,0x00,CC1101_MDMCFG1,0x02,CC1101_MDMCFG2,0x10,CC1101_MDMCFG3,0x93,CC1101_MDMCFG4,0x59,CC1101_DEVIATN,0x34,CC1101_MCSM0,0x18,CC1101_FOCCFG,0x16,CC1101_AGCCTRL0,0x80,CC1101_AGCCTRL1,0x58,CC1101_AGCCTRL2,0x87,CC1101_WORCTRL,0xFB,CC1101_FREND0,0x10,CC1101_FREND1,0x56,0,0,0xC0,0,0,0,0,0,0,0};
static const uint8_t sc_p_ook40[]={CC1101_IOCFG0,0x0D,CC1101_FIFOTHR,0x07,CC1101_PKTCTRL0,0x32,CC1101_FSCTRL1,0x06,CC1101_MDMCFG0,0x00,CC1101_MDMCFG1,0x00,CC1101_MDMCFG2,0x30,CC1101_MDMCFG3,0x93,CC1101_MDMCFG4,0x1A,CC1101_MCSM0,0x18,CC1101_FOCCFG,0x18,CC1101_AGCCTRL0,0x91,CC1101_AGCCTRL1,0x00,CC1101_AGCCTRL2,0x07,CC1101_WORCTRL,0xFB,CC1101_FREND0,0x11,CC1101_FREND1,0xB6,0,0,0,0xC0,0,0,0,0,0,0};
static const uint8_t sc_p_fsk40[]={CC1101_IOCFG0,0x0D,CC1101_FSCTRL1,0x06,CC1101_PKTCTRL0,0x32,CC1101_PKTCTRL1,0x04,CC1101_MDMCFG0,0x00,CC1101_MDMCFG1,0x02,CC1101_MDMCFG2,0x04,CC1101_MDMCFG3,0x93,CC1101_MDMCFG4,0x6A,CC1101_DEVIATN,0x41,CC1101_MCSM0,0x18,CC1101_FOCCFG,0x16,CC1101_AGCCTRL0,0x91,CC1101_AGCCTRL1,0x00,CC1101_AGCCTRL2,0x07,CC1101_WORCTRL,0xFB,CC1101_FREND0,0x10,CC1101_FREND1,0x56,0,0,0xC0,0,0,0,0,0,0,0};

static void sc_load_preset(const SubGhzDevice* dev, uint8_t mod) {
    switch(mod) {
    case SCModAM650: subghz_devices_load_preset(dev, FuriHalSubGhzPresetOok650Async, NULL); break;
    case SCModAM270: subghz_devices_load_preset(dev, FuriHalSubGhzPresetOok270Async, NULL); break;
    case SCModFM238: subghz_devices_load_preset(dev, FuriHalSubGhzPreset2FSKDev238Async, NULL); break;
    case SCModFM476: subghz_devices_load_preset(dev, FuriHalSubGhzPreset2FSKDev476Async, NULL); break;
    case SCModTPMS_FSK: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_p_tpms1); break;
    case SCModTPMS_OOK: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_p_tpms2); break;
    case SCModTPMS_GFSK: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_p_tpms3); break;
    case SCModOOK_40k: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_p_ook40); break;
    case SCModFSK_40k: subghz_devices_load_preset(dev, FuriHalSubGhzPresetCustom, (uint8_t*)sc_p_fsk40); break;
    default: subghz_devices_load_preset(dev, FuriHalSubGhzPresetOok650Async, NULL); break;
    }
}

static const uint32_t sc_hopper_freqs[] = {
    /* 300-348 */
    300000000,302757000,303000000,303875000,303900000,304250000,307000000,307500000,307800000,
    309000000,310000000,312000000,312100000,312200000,313000000,313850000,314000000,314350000,
    314980000,315000000,318000000,320000000,320150000,330000000,345000000,348000000,350000000,
    /* 387-464 */
    387000000,390000000,418000000,430000000,430500000,431000000,431500000,433075000,433220000,
    433420000,433657070,433889000,433920000,434075000,434176948,434190000,434390000,434420000,
    434620000,434775000,438900000,440175000,
    /* 779-928 */
    779000000,868350000,868400000,868460000,868800000,868950000,906400000,915000000,925000000,928000000};
#define SC_HOPPER_COUNT (sizeof(sc_hopper_freqs)/sizeof(sc_hopper_freqs[0]))

static const uint32_t sc_spec_freqs[] = {
    300000000,307000000,310000000,315000000,318000000,330000000,340000000,348000000,
    390000000,410000000,418000000,425000000,430000000,433920000,438000000,445000000,450000000,455000000,460000000,464000000,
    779000000,800000000,820000000,840000000,860000000,868000000,880000000,900000000,915000000,920000000,925000000,928000000};

// ============== Hit Log ==============
static void sc_hit_sort(SCHit* h, uint8_t n, SCSort mode) {
    for(uint8_t i = 1; i < n; i++) {
        SCHit t = h[i]; int8_t j = i - 1;
        while(j >= 0) {
            bool s = false;
            switch(mode) {
            case SCSortCount: s = h[j].count < t.count; break;
            case SCSortRSSI: s = h[j].rssi_max < t.rssi_max; break;
            case SCSortFreq: s = h[j].frequency > t.frequency; break;
            default: s = h[j].seq < t.seq; break;
            }
            if(!s) break;
            h[j + 1] = h[j]; j--;
        }
        h[j + 1] = t;
    }
}

static void sc_hit_add(SpectrumCheckApp* app, uint32_t freq, float rssi, const char* proto) {
    int8_t dbm = (int8_t)rssi;
    for(uint8_t i = 0; i < app->hit_count; i++) {
        uint32_t d = app->hits[i].frequency > freq ? app->hits[i].frequency - freq : freq - app->hits[i].frequency;
        if(d < 50000) {
            if(app->hits[i].count < 255) app->hits[i].count++;
            if(dbm > app->hits[i].rssi_max) app->hits[i].rssi_max = dbm;
            app->hits[i].seq = app->hit_seq++;
            if(proto && proto[0]) {
                if(!app->hits[i].protocol[0] || strcmp(app->hits[i].protocol, "BinRAW") == 0)
                    strncpy(app->hits[i].protocol, proto, 19);
            }
            return;
        }
    }
    if(app->hit_count < SC_HIT_LOG_SIZE) {
        SCHit* e = &app->hits[app->hit_count++];
        e->frequency = freq; e->count = 1; e->rssi_max = dbm; e->seq = app->hit_seq++;
        e->protocol[0] = 0;
        if(proto && proto[0]) strncpy(e->protocol, proto, 19);
    }
}

// ============== Signal Library ==============
static uint8_t sc_find_slot(SpectrumCheckApp* app) {
    if(app->signal_count < SC_SIGNAL_SLOTS) return app->signal_count;
    for(uint8_t i = SC_SIGNAL_SLOTS - 1; i > 0; i--)
        if(!app->signals[i].protocol_decoded) return i;
    return SC_SIGNAL_SLOTS - 1;
}

static void sc_analyze(SCSignal* s) {
    if(s->raw_count < 4) return;
    s->pulse_count = s->raw_count;
    s->total_duration_us = 0;
    s->min_pulse_us = UINT32_MAX;
    for(uint16_t i = 0; i < s->raw_count; i++) {
        uint32_t d = s->raw_data[i] > 0 ? (uint32_t)s->raw_data[i] : (uint32_t)(-s->raw_data[i]);
        s->total_duration_us += d;
        if(d < s->min_pulse_us && d > 50) s->min_pulse_us = d;
    }
    s->analyzed = true;
}

// ============== Save ==============
static void sc_save(SpectrumCheckApp* app, SCSignal* sig, const char* filename) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, "/ext/subghz");
    FuriString* p = furi_string_alloc();
    furi_string_printf(p, "/ext/subghz/%s.sub", filename);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    do {
        if(!flipper_format_file_open_always(ff, furi_string_get_cstr(p))) break;
        if(!flipper_format_write_header_cstr(ff, "Flipper SubGhz RAW File", 1)) break;
        if(!flipper_format_write_uint32(ff, "Frequency", &sig->frequency, 1)) break;
        if(!flipper_format_write_string_cstr(ff, "Preset", sc_preset_fnames[sig->modulation])) break;
        if(!flipper_format_write_string_cstr(ff, "Protocol", "RAW")) break;
        uint16_t w = 0;
        while(w < sig->raw_count) {
            uint16_t c = sig->raw_count - w;
            if(c > 128) c = 128;
            if(!flipper_format_write_int32(ff, "RAW_Data", &sig->raw_data[w], c)) break;
            w += c;
        }
    } while(0);
    flipper_format_free(ff);
    furi_string_free(p);
    furi_record_close(RECORD_STORAGE);
    notification_message(app->notifications, &sequence_success);
}

// ============== Worker Callbacks (run in worker thread) ==============
// Custom pair callback: stores raw samples AND feeds decoders
static void sc_raw_pair_cb(void* ctx, bool level, uint32_t duration) {
    SpectrumCheckApp* app = ctx;
    app->raw_buf[app->raw_write % SC_RAW_SAMPLES] = level ? (int32_t)duration : -(int32_t)duration;
    app->raw_write++;
    subghz_receiver_decode(app->receiver, level, duration);
    if(app->extra_receiver) subghz_receiver_decode(app->extra_receiver, level, duration);
}

static void sc_overrun_cb(void* ctx) {
    SpectrumCheckApp* app = ctx;
    subghz_receiver_reset(app->receiver);
    if(app->extra_receiver) subghz_receiver_reset(app->extra_receiver);
}

// Decode callback: fires from worker thread when protocol matched. Lock-free.
static void sc_decode_cb(SubGhzReceiver* rx, SubGhzProtocolDecoderBase* db, void* ctx) {
    UNUSED(rx);
    SpectrumCheckApp* app = ctx;
    if(app->pending_decode) return; // drop if previous not processed
    strncpy(app->pending_name, db->protocol->name, 31);
    FuriString* text = furi_string_alloc();
    subghz_protocol_decoder_base_get_string(db, text);
    strncpy(app->pending_str, furi_string_get_cstr(text), 127);
    furi_string_free(text);
    app->pending_freq = app->current_freq;
    app->pending_mod = app->current_mod;
    app->pending_decode = true;
}

// ============== Radio Control (ONLY called from main thread) ==============
static void sc_rx_start(SpectrumCheckApp* app) {
    if(app->rx_active) return;
    subghz_devices_start_async_rx(app->radio_device, subghz_worker_rx_callback, app->worker);
    subghz_worker_start(app->worker);
    app->rx_active = true;
}

static void sc_rx_end(SpectrumCheckApp* app) {
    if(!app->rx_active) return;
    subghz_worker_stop(app->worker);
    subghz_devices_stop_async_rx(app->radio_device);
    app->rx_active = false;
}

static void sc_set_freq_mod(SpectrumCheckApp* app, uint32_t freq, SCMod mod) {
    sc_rx_end(app);
    subghz_devices_idle(app->radio_device);
    sc_load_preset(app->radio_device, mod);
    subghz_devices_set_frequency(app->radio_device, freq);
    subghz_devices_flush_rx(app->radio_device);
    subghz_devices_set_rx(app->radio_device);
    app->current_freq = freq;
    app->current_mod = mod;
    subghz_receiver_reset(app->receiver);
    if(app->extra_receiver) subghz_receiver_reset(app->extra_receiver);
    app->raw_write = 0;
    sc_rx_start(app);
}

// ============== Tick Logic (called every 50ms from main loop) ==============
static void sc_update_noise_floor(SpectrumCheckApp* app, float rssi);
static bool sc_signal_present(SpectrumCheckApp* app, float rssi);

// Spectrum bandwidth presets (from external Spectrum Analyzer app)
static const char* sc_bw_names[] = {"Wide", "Med", "Narrow"};
static const uint8_t sc_bw_preset_wide[] = {
    CC1101_FSCTRL0, 0x00, CC1101_FSCTRL1, 0x12,
    CC1101_MDMCFG4, 0x17, // 650kHz BW
    CC1101_AGCCTRL2, 0xC0,
    CC1101_TEST2, 0x88, CC1101_TEST1, 0x31, CC1101_TEST0, 0x09,
    0, 0,
    0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t sc_bw_preset_med[] = {
    CC1101_FSCTRL0, 0x00, CC1101_FSCTRL1, 0x12,
    CC1101_MDMCFG4, 0x6C, // 270kHz BW
    CC1101_AGCCTRL2, 0xC0,
    CC1101_TEST2, 0x88, CC1101_TEST1, 0x31, CC1101_TEST0, 0x09,
    0, 0,
    0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t sc_bw_preset_narrow[] = {
    CC1101_FSCTRL0, 0x00, CC1101_FSCTRL1, 0x00,
    CC1101_MDMCFG4, 0xFC, // 58kHz BW
    CC1101_AGCCTRL0, 0x30, CC1101_AGCCTRL1, 0x00, CC1101_AGCCTRL2, 0x84,
    CC1101_TEST2, 0x88, CC1101_TEST1, 0x31, CC1101_TEST0, 0x09,
    0, 0,
    0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t* sc_bw_presets[] = {sc_bw_preset_wide, sc_bw_preset_med, sc_bw_preset_narrow};

// Band boundaries: 0-7=300MHz, 8-19=433MHz, 20-31=800MHz
static const uint8_t sc_band_start[] = {0, 0, 8, 20};
static const uint8_t sc_band_end[]   = {32, 8, 20, 32};
static const char* sc_band_names[]   = {"ALL", "300M", "433M", "800M"};

static void sc_tick_spectrum(SpectrumCheckApp* app) {
    sc_rx_end(app);
    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetCustom, (uint8_t*)sc_bw_presets[app->spec_bw]);

    uint8_t ch_start = sc_band_start[app->spec_band];
    uint8_t ch_end = sc_band_end[app->spec_band];

    float best = -200.0f;
    float worst = 0.0f;
    uint8_t best_ch = ch_start;
    for(uint8_t i = ch_start; i < ch_end; i++) {
        subghz_devices_set_frequency(app->radio_device, sc_spec_freqs[i]);
        subghz_devices_flush_rx(app->radio_device);
        subghz_devices_set_rx(app->radio_device);
        furi_delay_ms(app->spec_bw == 2 ? 4 : 2);
        float rssi = subghz_devices_get_rssi(app->radio_device);
        subghz_devices_idle(app->radio_device);

        int8_t db = (int8_t)rssi + 138;
        uint8_t val = (db < 0) ? 0 : ((db > 80) ? 80 : (uint8_t)db);
        if(val > app->spec_peak[i]) app->spec_peak[i] = val;
        if(rssi > best) { best = rssi; best_ch = i; }
        if(rssi < worst) worst = rssi;
    }
    sc_update_noise_floor(app, worst);
    if(++app->spec_decay >= 5) {
        app->spec_decay = 0;
        for(uint8_t i = ch_start; i < ch_end; i++)
            if(app->spec_peak[i] > 0) app->spec_peak[i]--;
    }
    if(best > app->spec_held_rssi || furi_get_tick() - app->spec_held_tick > 3000) {
        app->spec_held_rssi = best;
        app->spec_held_ch = best_ch;
        app->spec_held_tick = furi_get_tick();
    }
}

// ============== Coherent Signal Detection (ProtoView-inspired) ==============
// Scans raw_buf for runs of pulses with consistent timing (3 duration classes).
// Returns the length of the longest coherent run, and sets *te to the shortest pulse.
static uint16_t sc_detect_coherent(SpectrumCheckApp* app, uint32_t* te) {
    uint16_t count = app->raw_write > SC_RAW_SAMPLES ? SC_RAW_SAMPLES : app->raw_write;
    if(count < 20) return 0;

    uint16_t best_len = 0;
    uint32_t best_te = 0;

    // Duration classes: up to 3 per level (HIGH/LOW)
    uint32_t cls_dur[3][2]; // [class][level]
    uint16_t cls_cnt[3][2];

    uint16_t start = (app->raw_write > SC_RAW_SAMPLES) ? (app->raw_write % SC_RAW_SAMPLES) : 0;
    uint16_t run = 0;
    uint32_t short_dur = UINT32_MAX;

    memset(cls_dur, 0, sizeof(cls_dur));
    memset(cls_cnt, 0, sizeof(cls_cnt));

    for(uint16_t n = 0; n < count; n++) {
        uint16_t idx = (start + n) % SC_RAW_SAMPLES;
        int32_t v = app->raw_buf[idx];
        bool level = v > 0;
        uint32_t dur = level ? (uint32_t)v : (uint32_t)(-v);

        if(dur < 30 || dur > 4000) {
            // Out of range — end of coherent run
            if(run > best_len) { best_len = run; best_te = short_dur; }
            run = 0; short_dur = UINT32_MAX;
            memset(cls_dur, 0, sizeof(cls_dur));
            memset(cls_cnt, 0, sizeof(cls_cnt));
            continue;
        }

        // Try to match a class
        uint8_t lv = level ? 1 : 0;
        bool matched = false;
        for(uint8_t c = 0; c < 3; c++) {
            if(cls_cnt[c][lv] == 0) {
                cls_dur[c][lv] = dur;
                cls_cnt[c][lv] = 1;
                matched = true;
                break;
            }
            uint32_t avg = cls_dur[c][lv];
            uint32_t delta = dur > avg ? dur - avg : avg - dur;
            if(delta < avg / 5) { // 20% tolerance
                cls_dur[c][lv] = ((avg * cls_cnt[c][lv]) + dur) / (cls_cnt[c][lv] + 1);
                cls_cnt[c][lv]++;
                matched = true;
                break;
            }
        }

        if(!matched) {
            if(run > best_len) { best_len = run; best_te = short_dur; }
            run = 0; short_dur = UINT32_MAX;
            memset(cls_dur, 0, sizeof(cls_dur));
            memset(cls_cnt, 0, sizeof(cls_cnt));
            continue;
        }

        run++;
        if(dur < short_dur) short_dur = dur;
    }
    if(run > best_len) { best_len = run; best_te = short_dur; }

    *te = best_te;
    return best_len;
}

// Modulation presets to try when signal detected (most common first)
static const SCMod sc_try_mods[] = {SCModAM650, SCModAM270, SCModFM476, SCModFM238};
#define SC_TRY_MOD_COUNT 4

static void sc_tick_hopper(SpectrumCheckApp* app) {
    // === STATE: Worker running, staying on a signal ===
    if(app->rx_active && app->hopper_timeout > 0) {
        // Check RSSI — if signal still present, reset timeout (like firmware)
        float rssi = subghz_devices_get_rssi(app->radio_device);
        if(sc_signal_present(app, rssi)) {
            app->hopper_timeout = 20; // Reset: 1s from last signal presence
        } else {
            app->hopper_timeout--;
        }
        return;
    }

    // === STATE: Timeout expired — try next modulation or hop ===
    if(app->rx_active) {
        // Layer 3: Coherent signal detection before leaving
        if(app->was_on_signal && app->raw_write > 20) {
            bool already_captured = false;
            for(uint8_t i = 0; i < app->signal_count; i++) {
                uint32_t d = app->signals[i].frequency > app->current_freq ?
                    app->signals[i].frequency - app->current_freq : app->current_freq - app->signals[i].frequency;
                if(d < 50000) { already_captured = true; break; }
            }
            if(!already_captured) {
                uint32_t te = 0;
                uint16_t coherent_len = sc_detect_coherent(app, &te);
                if(coherent_len >= 18) {
                    uint8_t slot = sc_find_slot(app);
                    SCSignal* sig = &app->signals[slot];
                    memset(sig, 0, sizeof(SCSignal));
                    sig->frequency = app->current_freq;
                    sig->modulation = app->current_mod;
                    sig->raw_count = app->raw_write > SC_SIG_SAMPLES ? SC_SIG_SAMPLES : app->raw_write;
                    memcpy(sig->raw_data, app->raw_buf, sig->raw_count * sizeof(int32_t));
                    sig->min_pulse_us = te;
                    sig->pulse_count = coherent_len;
                    sig->analyzed = true;
                    if(slot >= app->signal_count && app->signal_count < SC_SIGNAL_SLOTS) app->signal_count++;
                }
            }
        }
        sc_rx_end(app);
        app->was_on_signal = false;
    }

    // === SCAN: Firmware-style hop through frequencies one at a time ===
    // Each tick we check ONE frequency. If signal present, start worker and stay.
    // This keeps ticks fast (~5ms) instead of blocking for 200ms scanning all freqs.
    subghz_devices_idle(app->radio_device);
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetOok650Async, NULL);

    // Scan a batch of frequencies per tick (8 at a time for responsiveness)
    float best_rssi = -200.0f;
    uint32_t best_freq = 0;
    uint8_t batch = 8;
    for(uint8_t b = 0; b < batch; b++) {
        uint32_t freq = sc_hopper_freqs[app->hopper_idx];
        subghz_devices_set_frequency(app->radio_device, freq);
        subghz_devices_flush_rx(app->radio_device);
        subghz_devices_set_rx(app->radio_device);
        furi_delay_ms(2);
        float rssi = subghz_devices_get_rssi(app->radio_device);
        subghz_devices_idle(app->radio_device);

        sc_update_noise_floor(app, rssi);

        if(rssi > best_rssi) {
            best_rssi = rssi;
            best_freq = freq;
        }
        app->hopper_idx = (app->hopper_idx + 1) % SC_HOPPER_COUNT;
    }

    app->current_freq = best_freq;

    // Nothing above noise floor — wait for next tick
    if(!sc_signal_present(app, best_rssi)) return;

    // === FINE SCAN: Pinpoint exact frequency ===
    static const uint8_t sc_fine_preset[] = {
        CC1101_FSCTRL0, 0x00, CC1101_FSCTRL1, 0x00,
        CC1101_MDMCFG4, 0xFC, // 58kHz BW
        CC1101_AGCCTRL0, 0x30, CC1101_AGCCTRL1, 0x00, CC1101_AGCCTRL2, 0x84,
        CC1101_TEST2, 0x88, CC1101_TEST1, 0x31, CC1101_TEST0, 0x09,
        0, 0,
        0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    subghz_devices_load_preset(app->radio_device, FuriHalSubGhzPresetCustom, (uint8_t*)sc_fine_preset);
    float fine_rssi = -200.0f;
    uint32_t fine_freq = best_freq;
    for(uint32_t f = best_freq - 300000; f <= best_freq + 300000; f += 20000) {
        if(!subghz_devices_is_frequency_valid(app->radio_device, f)) continue;
        subghz_devices_set_frequency(app->radio_device, f);
        subghz_devices_flush_rx(app->radio_device);
        subghz_devices_set_rx(app->radio_device);
        furi_delay_ms(2);
        float rssi = subghz_devices_get_rssi(app->radio_device);
        subghz_devices_idle(app->radio_device);
        if(rssi > fine_rssi) { fine_rssi = rssi; fine_freq = f; }
    }

    // === DECODE: Start worker, try modulations ===
    sc_hit_add(app, fine_freq, fine_rssi, NULL);
    app->was_on_signal = true;
    app->hopper_timeout = 20; // 1s dwell (matches firmware's 10 ticks × 100ms)

    // Pick modulation: separate counter from freq index
    app->mod_rotation = (app->mod_rotation + 1) % SC_TRY_MOD_COUNT;
    SCMod mod = sc_try_mods[app->mod_rotation];
    sc_set_freq_mod(app, fine_freq, mod);
}

// Adaptive noise floor estimation (asymmetric: tracks down fast, up slow)
static void sc_update_noise_floor(SpectrumCheckApp* app, float rssi) {
    if(app->noise_floor <= -120.0f) {
        app->noise_floor = rssi; // First real reading replaces init value
    } else if(rssi < app->noise_floor) {
        // Below floor: track down quickly
        app->noise_floor = app->noise_floor * 0.9f + rssi * 0.1f;
    } else if(rssi < app->noise_floor + 6.0f) {
        // Near floor (not a signal): allow slow upward drift
        app->noise_floor = app->noise_floor * 0.995f + rssi * 0.005f;
    }
    // Readings well above floor (signals) are ignored
}

// Is signal clearly present? (10dB above noise floor)
static bool sc_signal_present(SpectrumCheckApp* app, float rssi) {
    return rssi > app->noise_floor + 10.0f;
}

static void sc_tick_locked(SpectrumCheckApp* app) {
    if(!app->rx_active || app->current_freq != app->locked_freq) {
        sc_set_freq_mod(app, app->locked_freq, sc_try_mods[app->locked_mod_idx % SC_TRY_MOD_COUNT]);
        app->hopper_timeout = 40; // 2s initial dwell
        return;
    }
    float rssi = subghz_devices_get_rssi(app->radio_device);
    sc_update_noise_floor(app, rssi);

    if(sc_signal_present(app, rssi)) {
        app->hopper_timeout = 40; // Reset: 2s from last signal presence
        sc_hit_add(app, app->locked_freq, rssi, NULL); // Keep hit log updated
        return;
    }
    if(app->hopper_timeout > 0) {
        app->hopper_timeout--;
        return;
    }
    // 2s of silence — try next modulation
    app->locked_mod_idx++;
    sc_set_freq_mod(app, app->locked_freq, sc_try_mods[app->locked_mod_idx % SC_TRY_MOD_COUNT]);
    app->hopper_timeout = 40;
}

static void sc_tick_camp(SpectrumCheckApp* app) {
    // Ensure worker is running on the right freq+mod (only set once, not every tick)
    SCMod target_mod = sc_try_mods[app->camp_mod_idx % SC_TRY_MOD_COUNT];
    if(!app->rx_active) {
        sc_set_freq_mod(app, app->locked_freq, target_mod);
        if(!app->camp_start_tick) app->camp_start_tick = furi_get_tick();
    }
    float rssi = subghz_devices_get_rssi(app->radio_device);
    sc_update_noise_floor(app, rssi);

    // Log hits when signal present (so freq analyzer updates in real-time)
    if(sc_signal_present(app, rssi)) {
        sc_hit_add(app, app->locked_freq, rssi, NULL);
    }

    // Coherent signal detection: check raw buffer periodically for non-protocol signals
    if(app->raw_write > 40) {
        static uint16_t last_check = 0;
        if(app->raw_write - last_check > 200) {
            last_check = app->raw_write;
            uint32_t te = 0;
            uint16_t coherent_len = sc_detect_coherent(app, &te);
            if(coherent_len >= 18) {
                // Check if we already have this freq captured
                bool already = false;
                for(uint8_t i = 0; i < app->signal_count; i++) {
                    uint32_t d = app->signals[i].frequency > app->locked_freq ?
                        app->signals[i].frequency - app->locked_freq : app->locked_freq - app->signals[i].frequency;
                    if(d < 50000) { already = true; break; }
                }
                if(!already) {
                    uint8_t slot = sc_find_slot(app);
                    SCSignal* sig = &app->signals[slot];
                    memset(sig, 0, sizeof(SCSignal));
                    sig->frequency = app->locked_freq;
                    sig->modulation = app->current_mod;
                    sig->raw_count = app->raw_write > SC_SIG_SAMPLES ? SC_SIG_SAMPLES : app->raw_write;
                    memcpy(sig->raw_data, app->raw_buf, sig->raw_count * sizeof(int32_t));
                    sig->min_pulse_us = te;
                    sig->pulse_count = coherent_len;
                    sig->analyzed = true;
                    if(slot >= app->signal_count && app->signal_count < SC_SIGNAL_SLOTS) app->signal_count++;
                    sc_hit_add(app, app->locked_freq, rssi, "Coherent");
                }
            }
        }
    }
}

// Process pending decode from worker thread
static void sc_process_decode(SpectrumCheckApp* app) {
    if(!app->pending_decode) return;
    sc_hit_add(app, app->pending_freq, app->noise_floor + 10.0f, app->pending_name);
    // Dedup: find existing slot for same freq+protocol, OR upgrade BinRAW/Unknown on same freq
    uint8_t slot = 0xFF;
    for(uint8_t i = 0; i < app->signal_count; i++) {
        uint32_t d = app->signals[i].frequency > app->pending_freq ?
            app->signals[i].frequency - app->pending_freq : app->pending_freq - app->signals[i].frequency;
        if(d < 50000) {
            if(strcmp(app->signals[i].protocol_name, app->pending_name) == 0) {
                slot = i; break; // Exact match — update in place
            }
            // Upgrade: specific protocol replaces BinRAW or unidentified
            if(!app->signals[i].protocol_decoded ||
               strcmp(app->signals[i].protocol_name, "BinRAW") == 0) {
                slot = i; break;
            }
        }
    }
    if(slot == 0xFF) slot = sc_find_slot(app);
    SCSignal* sig = &app->signals[slot];
    memset(sig, 0, sizeof(SCSignal));
    sig->frequency = app->pending_freq;
    sig->modulation = app->pending_mod;
    sig->raw_count = app->raw_write > SC_SIG_SAMPLES ? SC_SIG_SAMPLES : app->raw_write;
    memcpy(sig->raw_data, app->raw_buf, sig->raw_count * sizeof(int32_t));
    strncpy(sig->protocol_name, app->pending_name, 31);
    strncpy(sig->decoded_string, app->pending_str, 127);
    sig->protocol_decoded = true;
    sc_analyze(sig);
    if(slot >= app->signal_count && app->signal_count < SC_SIGNAL_SLOTS) app->signal_count++;
    app->signal_selected = slot;
    app->pending_decode = false;
    // Update camp display
    if(app->current_view == SCViewCamp)
        strncpy(app->camp_last_proto, app->pending_name, 31);
}

// ============== Drawing ==============
static void sc_draw_status(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[48];
    canvas_set_font(canvas, FontSecondary);
    if(app->radio_state == SCRadioLocked) {
        SCMod cur_mod = sc_try_mods[app->locked_mod_idx % SC_TRY_MOD_COUNT];
        snprintf(buf, sizeof(buf), "LOCK %ld.%03ld %s %d/%d NF:%.0f",
            app->locked_freq / 1000000 % 1000, app->locked_freq / 1000 % 1000,
            sc_mod_names[cur_mod],
            (app->locked_mod_idx % SC_TRY_MOD_COUNT) + 1, SC_TRY_MOD_COUNT, (double)app->noise_floor);
        canvas_draw_str(canvas, 0, 63, buf);
    } else {
        // Hopping: progress bar + hit count
        uint8_t bar_fill = (app->hopper_idx * 58) / SC_HOPPER_COUNT;
        canvas_draw_frame(canvas, 0, 56, 60, 7);
        if(bar_fill > 0) canvas_draw_box(canvas, 1, 57, bar_fill, 5);
        snprintf(buf, sizeof(buf), "%dh %ds NF:%.0f",
            app->hit_count, app->signal_count, (double)app->noise_floor);
        canvas_draw_str(canvas, 63, 63, buf);
    }
}

static void sc_draw_spectrum(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[48];
    uint8_t ch_start = sc_band_start[app->spec_band];
    uint8_t ch_end = sc_band_end[app->spec_band];
    uint8_t ch_count = ch_end - ch_start;
    uint8_t bar_w = 128 / ch_count; // wider bars when focused
    if(bar_w < 3) bar_w = 3;

    uint8_t noise = 80;
    for(uint8_t i = ch_start; i < ch_end; i++)
        if(app->spec_peak[i] < noise) noise = app->spec_peak[i];

    for(uint8_t i = ch_start; i < ch_end; i++) {
        uint8_t x = (i - ch_start) * bar_w;
        int8_t val = (int8_t)app->spec_peak[i] - (int8_t)noise;
        if(val < 0) val = 0;
        uint8_t h = val * 42 / 50;
        if(h > 42) h = 42;
        if(h > 0) canvas_draw_box(canvas, x, 52 - h, bar_w - 1, h);
        if(i == app->spec_held_ch && app->spec_held_rssi > -90.0f)
            canvas_draw_frame(canvas, x > 0 ? x - 1 : 0, 52 - h - 2, bar_w + 1, h + 3);
    }
    // Band separators (only in ALL mode)
    if(app->spec_band == 0) {
        for(uint8_t y = 10; y < 52; y += 2) {
            canvas_draw_dot(canvas, 8 * bar_w, y);
            canvas_draw_dot(canvas, 20 * bar_w, y);
        }
    }
    // Decoded signal markers
    for(uint8_t s = 0; s < app->signal_count; s++) {
        if(!app->signals[s].protocol_decoded) continue;
        uint32_t f = app->signals[s].frequency;
        uint8_t cl = ch_start; uint32_t md = UINT32_MAX;
        for(uint8_t i = ch_start; i < ch_end; i++) {
            uint32_t d = f > sc_spec_freqs[i] ? f - sc_spec_freqs[i] : sc_spec_freqs[i] - f;
            if(d < md) { md = d; cl = i; }
        }
        canvas_draw_dot(canvas, (cl - ch_start) * bar_w + bar_w / 2, 54);
    }
    // Top bar: single line with all info
    if(app->spec_held_rssi > -110.0f) {
        uint32_t pf = sc_spec_freqs[app->spec_held_ch];
        snprintf(buf, sizeof(buf), "%s %s %.0f %ld.%02ld",
            sc_band_names[app->spec_band], sc_bw_names[app->spec_bw],
            (double)app->spec_held_rssi, pf / 1000000, (pf / 10000) % 100);
    } else {
        snprintf(buf, sizeof(buf), "%s %s  --",
            sc_band_names[app->spec_band], sc_bw_names[app->spec_bw]);
    }
    canvas_draw_str(canvas, 0, 7, buf);
}

static void sc_draw_freq(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];
    uint8_t list_y;
    if(app->radio_state == SCRadioLocked) {
        canvas_set_font(canvas, FontPrimary);
        SCMod lmod = sc_try_mods[app->locked_mod_idx % SC_TRY_MOD_COUNT];
        snprintf(buf, sizeof(buf), "LOCK %ld.%03ld [%s]",
            app->locked_freq / 1000000 % 1000, app->locked_freq / 1000 % 1000,
            sc_mod_names[lmod]);
        canvas_draw_box(canvas, 0, 0, 128, 12);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str(canvas, 2, 10, buf);
        canvas_set_color(canvas, ColorBlack);
        canvas_set_font(canvas, FontSecondary);
        list_y = 22;
    } else {
        canvas_set_font(canvas, FontSecondary);
        snprintf(buf, sizeof(buf), "T:%.0f %dhits [%s]", (double)app->trigger, app->hit_count, sc_sort_names[app->hit_sort]);
        canvas_draw_str(canvas, 0, 7, buf);
        list_y = 14;
    }
    // Hit list
    canvas_set_font(canvas, FontKeyboard);
    uint8_t visible = (54 - list_y) / 8;
    uint8_t start = app->hit_cursor >= visible ? app->hit_cursor - visible + 1 : 0;
    for(uint8_t i = 0; i < visible && (start + i) < app->hit_count; i++) {
        SCHit* h = &app->hits[start + i];
        uint8_t y = list_y + i * 8;
        if((start + i) == app->hit_cursor) {
            canvas_draw_box(canvas, 0, y, 128, 8);
            canvas_set_color(canvas, ColorWhite);
        }
        if(h->protocol[0])
            snprintf(buf, sizeof(buf), "%ld.%03ld %dx %ddBm %s",
                h->frequency / 1000000 % 1000, h->frequency / 1000 % 1000, h->count, h->rssi_max, h->protocol);
        else
            snprintf(buf, sizeof(buf), "%ld.%03ld %dx %ddBm",
                h->frequency / 1000000 % 1000, h->frequency / 1000 % 1000, h->count, h->rssi_max);
        canvas_draw_str(canvas, 1, y + 7, buf);
        canvas_set_color(canvas, ColorBlack);
    }
}

static void sc_draw_decoder(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];
    canvas_set_font(canvas, FontSecondary);
    if(app->signal_count == 0) {
        canvas_draw_str(canvas, 4, 20, "No decoded signals yet.");
        canvas_draw_str(canvas, 4, 32, "Hopper auto-captures signals.");
        canvas_draw_str(canvas, 4, 44, "L/R: browse  LongOK: save");
        return;
    }
    SCSignal* sig = &app->signals[app->signal_selected];
    // Header: slot, freq, mod, quality indicator
    const char* type_icon = sig->protocol_decoded ? (strcmp(sig->protocol_name, "BinRAW") == 0 ? "BIN" : "DEC") : "RAW";
    snprintf(buf, sizeof(buf), "%d/%d %s %ld.%03ld %s %dp",
        app->signal_selected + 1, app->signal_count, type_icon,
        sig->frequency / 1000000, (sig->frequency / 1000) % 1000,
        sc_mod_names[sig->modulation], sig->pulse_count);
    canvas_draw_str(canvas, 0, 7, buf);
    if(sig->protocol_decoded) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 0, 19, sig->protocol_name);
        canvas_set_font(canvas, FontSecondary);
        // Scrollable decoded text
        const char* p = sig->decoded_string;
        // Skip lines based on decoder_scroll
        for(uint8_t skip = 0; skip < app->decoder_scroll && *p; skip++) {
            const char* nl = strchr(p, '\n');
            if(nl) p = nl + 1; else { p += strlen(p); break; }
        }
        uint8_t y = 29;
        for(uint8_t l = 0; l < 4 && *p && y <= 54; l++) {
            const char* nl = strchr(p, '\n');
            uint8_t len = nl ? (uint8_t)(nl - p) : strlen(p);
            if(len > 42) len = 42;
            char tmp[43]; memcpy(tmp, p, len); tmp[len] = 0;
            canvas_draw_str(canvas, 0, y, tmp);
            y += 8; p += len; if(*p == '\n') p++;
        }
        // Scroll indicator
        if(app->decoder_scroll > 0) canvas_draw_str(canvas, 122, 22, "^");
        if(*p) canvas_draw_str(canvas, 122, 54, "v");
    } else if(sig->analyzed) {
        snprintf(buf, sizeof(buf), "Coherent: %d pulses", sig->pulse_count);
        canvas_draw_str(canvas, 0, 22, buf);
        snprintf(buf, sizeof(buf), "Min pulse: %ldus", sig->min_pulse_us);
        canvas_draw_str(canvas, 0, 32, buf);
        snprintf(buf, sizeof(buf), "Samples: %d", sig->raw_count);
        canvas_draw_str(canvas, 0, 42, buf);
    }
}

static void sc_draw_camp(Canvas* canvas, SpectrumCheckApp* app) {
    char buf[64];
    SCMod mod = sc_try_mods[app->camp_mod_idx % SC_TRY_MOD_COUNT];
    canvas_set_font(canvas, FontPrimary);
    if(app->radio_state != SCRadioLocked) {
        canvas_draw_str(canvas, 0, 12, "CAMP MODE");
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 0, 26, "No frequency locked.");
        canvas_draw_str(canvas, 0, 38, "Go to Spectrum or Freq Analyzer,");
        canvas_draw_str(canvas, 0, 50, "press OK to lock a frequency.");
    } else {
        uint32_t elapsed = (furi_get_tick() - app->camp_start_tick) / 1000;
        uint32_t mins = elapsed / 60, secs = elapsed % 60;
        snprintf(buf, sizeof(buf), "CAMP %ld.%03ld [%s]",
            app->locked_freq / 1000000, (app->locked_freq / 1000) % 1000, sc_mod_names[mod]);
        canvas_draw_str(canvas, 0, 12, buf);
        canvas_set_font(canvas, FontSecondary);
        snprintf(buf, sizeof(buf), "Time: %ld:%02ld  Sig:%d", mins, secs, app->signal_count);
        canvas_draw_str(canvas, 0, 24, buf);
        // RSSI bar
        float rssi = app->rx_active ? subghz_devices_get_rssi(app->radio_device) : -127.0f;
        int8_t bar = (int8_t)(rssi + 100);
        if(bar < 0) bar = 0;
        if(bar > 60) bar = 60;
        canvas_draw_frame(canvas, 0, 28, 62, 8);
        if(bar > 0) canvas_draw_box(canvas, 1, 29, bar, 6);
        snprintf(buf, sizeof(buf), "%.0fdBm", (double)rssi);
        canvas_draw_str(canvas, 66, 35, buf);
        // Last decode
        if(app->camp_last_proto[0]) {
            snprintf(buf, sizeof(buf), "Decoded: %s", app->camp_last_proto);
            canvas_draw_str(canvas, 0, 46, buf);
        } else {
            canvas_draw_str(canvas, 0, 46, "Waiting for signal...");
        }
        canvas_draw_str(canvas, 0, 56, "L/R:mod  LongOK:save");
    }
}

static void sc_draw_cb(Canvas* canvas, void* ctx) {
    SpectrumCheckApp* app = ctx;
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
    switch(app->current_view) {
    case SCViewSpectrum: sc_draw_spectrum(canvas, app); break;
    case SCViewFreqAnalyzer: sc_draw_freq(canvas, app); break;
    case SCViewCamp: sc_draw_camp(canvas, app); break;
    case SCViewDecoder: sc_draw_decoder(canvas, app); break;
    default: break;
    }
    if(app->current_view != SCViewCamp) sc_draw_status(canvas, app);
}

// ============== Input ==============
static void sc_input_cb(InputEvent* ev, void* ctx) {
    furi_message_queue_put(((SpectrumCheckApp*)ctx)->event_queue, ev, FuriWaitForever);
}

static void sc_handle_input(SpectrumCheckApp* app, InputEvent* ev) {
    if(ev->type != InputTypeShort && ev->type != InputTypeLong && ev->type != InputTypeRepeat) return;
    switch(ev->key) {
    case InputKeyUp:
        if(app->current_view == SCViewCamp) { app->camp_start_tick = 0; app->camp_last_proto[0] = 0; }
        app->current_view = app->current_view == 0 ? SCViewCount - 1 : app->current_view - 1;
        app->decoder_scroll = 0;
        break;
    case InputKeyDown:
        if(app->current_view == SCViewSpectrum && app->spec_held_rssi > -95.0f) {
            uint32_t best_freq = sc_spec_freqs[app->spec_held_ch];
            for(uint8_t i = 0; i < SC_HOPPER_COUNT; i++) {
                if(sc_hopper_freqs[i] == best_freq) { app->hopper_idx = i; break; }
            }
        }
        if(app->current_view == SCViewCamp) { app->camp_start_tick = 0; app->camp_last_proto[0] = 0; }
        app->current_view = (app->current_view + 1) % SCViewCount;
        app->decoder_scroll = 0;
        break;
    case InputKeyLeft:
        if(app->current_view == SCViewSpectrum) {
            if(ev->type == InputTypeLong) { // no repeat — single switch per long press
                app->spec_band = app->spec_band == 0 ? 3 : app->spec_band - 1;
                memset(app->spec_peak, 0, sizeof(app->spec_peak));
                app->spec_held_rssi = -127.0f;
            } else if(ev->type == InputTypeShort) {
                if(app->spec_bw > 0) app->spec_bw--;
                memset(app->spec_peak, 0, sizeof(app->spec_peak));
            }
        } else if(app->current_view == SCViewFreqAnalyzer) {
            if(ev->type == InputTypeLong || ev->type == InputTypeRepeat) {
                app->trigger -= SC_TRIGGER_STEP; if(app->trigger < SC_RSSI_MIN) app->trigger = SC_RSSI_MIN;
            } else { if(app->hit_cursor > 0) app->hit_cursor--; }
        } else if(app->current_view == SCViewCamp) {
            if(app->camp_mod_idx > 0) app->camp_mod_idx--; else app->camp_mod_idx = SC_TRY_MOD_COUNT - 1;
            if(app->radio_state == SCRadioLocked) {
                sc_set_freq_mod(app, app->locked_freq, sc_try_mods[app->camp_mod_idx % SC_TRY_MOD_COUNT]);
                app->camp_start_tick = furi_get_tick();
                app->camp_last_proto[0] = 0;
            }
        } else if(app->current_view == SCViewDecoder) {
            if(app->signal_selected > 0) { app->signal_selected--; app->decoder_scroll = 0; }
        }
        break;
    case InputKeyRight:
        if(app->current_view == SCViewSpectrum) {
            if(ev->type == InputTypeLong) { // no repeat — single switch per long press
                app->spec_band = (app->spec_band + 1) % 4;
                memset(app->spec_peak, 0, sizeof(app->spec_peak));
                app->spec_held_rssi = -127.0f;
            } else if(ev->type == InputTypeShort) {
                if(app->spec_bw < 2) app->spec_bw++;
                memset(app->spec_peak, 0, sizeof(app->spec_peak));
            }
        } else if(app->current_view == SCViewFreqAnalyzer) {
            if(ev->type == InputTypeLong || ev->type == InputTypeRepeat) {
                app->trigger += SC_TRIGGER_STEP; if(app->trigger > SC_RSSI_MAX) app->trigger = SC_RSSI_MAX;
            } else { if(app->hit_cursor + 1 < app->hit_count) app->hit_cursor++; }
        } else if(app->current_view == SCViewCamp) {
            app->camp_mod_idx = (app->camp_mod_idx + 1) % SC_TRY_MOD_COUNT;
            if(app->radio_state == SCRadioLocked) {
                sc_set_freq_mod(app, app->locked_freq, sc_try_mods[app->camp_mod_idx % SC_TRY_MOD_COUNT]);
                app->camp_start_tick = furi_get_tick();
                app->camp_last_proto[0] = 0;
            }
        } else if(app->current_view == SCViewDecoder) {
            if(app->signal_selected + 1 < app->signal_count) { app->signal_selected++; app->decoder_scroll = 0; }
        }
        break;
    case InputKeyOk:
        if(ev->type == InputTypeLong) {
            if(app->current_view == SCViewFreqAnalyzer) {
                app->hit_sort = (app->hit_sort + 1) % SCSortModes;
                sc_hit_sort(app->hits, app->hit_count, app->hit_sort);
            } else if(app->signal_count > 0 && (app->current_view == SCViewDecoder || app->current_view == SCViewCamp)) {
                // Long OK: save with keyboard
                SCSignal* s = &app->signals[app->signal_selected];
                if(s->raw_count > 0) {
                    app->save_slot = app->signal_selected;
                    // Auto-generate default filename
                    if(s->protocol_decoded && strcmp(s->protocol_name, "BinRAW") != 0)
                        snprintf(app->save_filename, sizeof(app->save_filename), "%s_%ld", s->protocol_name, s->frequency / 1000);
                    else
                        snprintf(app->save_filename, sizeof(app->save_filename), "SC_%ld_%s", s->frequency / 1000, sc_mod_names[s->modulation]);
                    app->show_keyboard = true;
                }
            }
        } else if(ev->type == InputTypeShort) {
            if(app->current_view == SCViewDecoder) {
                // OK short: scroll decoded text
                if(app->signal_count > 0) app->decoder_scroll++;
            } else if(app->current_view == SCViewSpectrum || app->current_view == SCViewFreqAnalyzer) {
                if(app->radio_state == SCRadioLocked) {
                    app->radio_state = SCRadioHopping;
                } else {
                    uint32_t freq = 0;
                    if(app->current_view == SCViewSpectrum && app->spec_held_ch < SC_SPEC_CH)
                        freq = sc_spec_freqs[app->spec_held_ch];
                    else if(app->current_view == SCViewFreqAnalyzer && app->hit_count > 0)
                        freq = app->hits[app->hit_cursor].frequency;
                    if(freq) {
                        app->locked_freq = freq;
                        app->locked_mod_idx = 0;
                        app->radio_state = SCRadioLocked;
                        app->hopper_timeout = 40;
                    }
                }
            }
        }
        break;
    default: break;
    }
}

// ============== Keyboard Save (ProtoView pattern) ==============
static void sc_keyboard_done_cb(void* ctx) {
    SpectrumCheckApp* app = ctx;
    // Save the signal with the user-chosen filename
    if(app->save_slot < app->signal_count && app->save_filename[0]) {
        sc_save(app, &app->signals[app->save_slot], app->save_filename);
    }
    // Stop the view dispatcher to return to our main loop
    view_dispatcher_stop(app->view_dispatcher);
}

static bool sc_keyboard_nav_cb(void* ctx) {
    UNUSED(ctx);
    return false; // Returning false stops the view dispatcher (user pressed Back)
}

static void sc_show_keyboard(SpectrumCheckApp* app) {
    // Stop radio while in keyboard
    sc_rx_end(app);

    // Swap ViewPort for ViewDispatcher+TextInput
    gui_remove_view_port(app->gui, app->view_port);

    ViewDispatcher* vd = view_dispatcher_alloc();
    TextInput* ti = text_input_alloc();
    view_dispatcher_set_navigation_event_callback(vd, sc_keyboard_nav_cb);
    view_dispatcher_set_event_callback_context(vd, app);
    view_dispatcher_add_view(vd, 0, text_input_get_view(ti));
    view_dispatcher_switch_to_view(vd, 0);

    text_input_set_header_text(ti, "Name signal");
    text_input_set_result_callback(ti, sc_keyboard_done_cb, app, app->save_filename, sizeof(app->save_filename), false);

    app->view_dispatcher = vd;
    view_dispatcher_attach_to_gui(vd, app->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_run(vd); // Blocks until done or back

    // Cleanup and restore
    view_dispatcher_remove_view(vd, 0);
    text_input_free(ti);
    view_dispatcher_free(vd);
    app->view_dispatcher = NULL;

    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    app->show_keyboard = false;
}

// ============== App Entry ==============
int32_t spectrum_check_app(void* p) {
    UNUSED(p);
    SpectrumCheckApp* app = malloc(sizeof(SpectrumCheckApp));
    memset(app, 0, sizeof(SpectrumCheckApp));
    app->trigger = -90.0f;
    app->noise_floor = -120.0f; // Will adapt quickly on first scan
    app->current_freq = 433920000;
    app->current_mod = SCModAM650;
    app->radio_state = SCRadioHopping;
    app->spec_held_rssi = -127.0f;
    app->running = true;

    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Radio init
    subghz_devices_init();
    app->radio_device = radio_device_loader_set(NULL, SubGhzRadioDeviceTypeInternal);

    // Main protocol registry (50+ protocols)
    app->environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(app->environment, (void*)&subghz_protocol_registry);
    app->receiver = subghz_receiver_alloc_init(app->environment);
    subghz_receiver_set_filter(app->receiver, SubGhzProtocolFlag_Decodable | SubGhzProtocolFlag_BinRAW);
    subghz_receiver_set_rx_callback(app->receiver, sc_decode_cb, app);
    // Get BinRAW decoder for RSSI feeding
    SubGhzProtocolDecoderBase* bin_raw_base =
        subghz_receiver_search_decoder_base_by_name(app->receiver, SUBGHZ_PROTOCOL_BIN_RAW_NAME);
    app->bin_raw_decoder = (SubGhzProtocolDecoderBinRAW*)bin_raw_base;

    // Extra protocol registry (weather/tpms/pocsag)
    app->extra_environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(app->extra_environment, (void*)&sc_extra_protocol_registry);
    app->extra_receiver = subghz_receiver_alloc_init(app->extra_environment);
    subghz_receiver_set_filter(app->extra_receiver, SubGhzProtocolFlag_Decodable);
    subghz_receiver_set_rx_callback(app->extra_receiver, sc_decode_cb, app);

    // Worker: custom pair callback for raw capture + decode
    app->worker = subghz_worker_alloc();
    subghz_worker_set_overrun_callback(app->worker, (SubGhzWorkerOverrunCallback)sc_overrun_cb);
    subghz_worker_set_pair_callback(app->worker, (SubGhzWorkerPairCallback)sc_raw_pair_cb);
    subghz_worker_set_context(app->worker, app);

    // GUI
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, sc_draw_cb, app);
    view_port_input_callback_set(app->view_port, sc_input_cb, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    // Main loop — 50ms tick drives everything
    InputEvent event;
    while(app->running) {
        if(furi_message_queue_get(app->event_queue, &event, 50) == FuriStatusOk) {
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                app->running = false;
            } else {
                sc_handle_input(app, &event);
            }
        }
        // Tick: radio management based on current view and state
        if(app->current_view == SCViewCamp && app->radio_state == SCRadioLocked) {
            sc_tick_camp(app);
        } else if(app->radio_state == SCRadioLocked) {
            sc_tick_locked(app);
        } else if(app->current_view == SCViewSpectrum) {
            sc_tick_spectrum(app);
        } else {
            sc_tick_hopper(app);
        }
        // Process any pending decode from worker
        sc_process_decode(app);
        // Feed RSSI to BinRAW decoder (needed for signal boundary detection)
        if(app->rx_active && app->bin_raw_decoder) {
            float rssi = subghz_devices_get_rssi(app->radio_device);
            subghz_protocol_decoder_bin_raw_data_input_rssi(app->bin_raw_decoder, rssi);
        }
        view_port_update(app->view_port);

        // Keyboard save flow (blocks main loop while keyboard is shown)
        if(app->show_keyboard) {
            sc_show_keyboard(app);
        }
    }

    // Cleanup
    sc_rx_end(app);
    subghz_devices_idle(app->radio_device);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);
    subghz_worker_free(app->worker);
    subghz_receiver_free(app->receiver);
    subghz_environment_free(app->environment);
    subghz_receiver_free(app->extra_receiver);
    subghz_environment_free(app->extra_environment);
    radio_device_loader_end(app->radio_device);
    subghz_devices_deinit();
    furi_record_close(RECORD_NOTIFICATION);
    furi_message_queue_free(app->event_queue);
    free(app);
    return 0;
}
