/*
 * Neighborhood v4 — 433.92 MHz device awareness & replay
 * Menu-first: Scan, My Remotes, Known Devices, Settings
 */
#include "neighborhood_remote.h"
#define TAG "Neighborhood"

// ============== Classification ==============

static NRProto nr_classify(uint16_t te, uint16_t bits, uint8_t* d, uint8_t len) {
    if(len >= 3) {
        uint8_t ff = 0;
        for(uint8_t i = 0; i < len; i++) if(d[i] == 0xFF) ff++;
        if(ff > len / 3 && te < 70) return NRProtoFSK;
    }
    if(te >= 500 && te <= 750 && bits >= 30) return NRProtoNexusTH;
    if(te >= 220 && te <= 360 && bits >= 64 && bits <= 68) return NRProtoKeeloq;
    if(te >= 110 && te <= 210 && bits >= 30) return NRProtoHoneywell;
    if(te >= 70 && te <= 84 && bits >= 50) return NRProtoHoneywell; // half-bit Manchester
    if(te >= 175 && te <= 215 && bits >= 16 && bits <= 50) return NRProtoPT2262;
    if(te >= 105 && te <= 130 && bits >= 20 && bits <= 80) return NRProtoEV1527;
    return NRProtoBinRAW;
}

static uint32_t nr_dev_id(NRProto p, uint8_t* d, uint8_t len, uint16_t te) {
    if(p == NRProtoPT2262 && len >= 3) return d[len - 3];
    if(p == NRProtoEV1527 && len >= 3)
        return (((uint32_t)d[0]<<16)|((uint32_t)d[1]<<8)|d[2]) >> 4;
    if(p == NRProtoKeeloq && len >= 8) {
        uint32_t sn = (((uint32_t)d[4]&0xF)<<24)|((uint32_t)d[5]<<16)|((uint32_t)d[6]<<8)|d[7];
        return sn >> 4;
    }
    if(p == NRProtoHoneywell) return 0x5800;
    if(p == NRProtoFSK) return 0xF5C0;
    if(p == NRProtoNexusTH && len >= 1) return 0xE000 | d[0];
    // Group OOK meter variants (TE 90-109) into single device
    if(p == NRProtoBinRAW && te >= 90 && te <= 109) return 0xB109;
    return 0xB100 | ((te / 10) & 0xFF);
}

static void nr_sig_label(NRProto p, uint8_t* d, uint8_t len, char* out, uint8_t sz) {
    if(p == NRProtoPT2262 && len >= 3)
        snprintf(out, sz, "Cmd:%02X", (unsigned)d[len-1]);
    else if(p == NRProtoEV1527 && len >= 3) {
        uint8_t cmd = d[2] & 0xF;
        const char* hint = cmd == 0xF ? "Alrm" : cmd == 0xE ? "Door" :
            cmd == 0x8 ? "PIR" : cmd == 0x2 ? "BtnB" :
            cmd == 0x4 ? "BtnC" : cmd == 0x1 ? "BtnA" : "Sens";
        uint32_t addr = (((uint32_t)d[0]<<16)|((uint32_t)d[1]<<8)|d[2]) >> 4;
        snprintf(out, sz, "%05lX %s", (unsigned long)addr, hint);
    } else if(p == NRProtoKeeloq && len >= 8) {
        uint32_t sn = (((uint32_t)d[4]&0xF)<<24)|((uint32_t)d[5]<<16)|
                      ((uint32_t)d[6]<<8)|d[7];
        snprintf(out, sz, "S%u %06lX", (unsigned)(d[7]&0xF), (unsigned long)(sn>>4));
    } else if(p == NRProtoHoneywell && len >= 4) {
        // Manchester decode first 64 raw bits → 32 decoded bits
        uint8_t dec[4] = {0};
        uint8_t di = 0;
        for(uint8_t i = 0; i < len * 8 - 1 && di < 32; i += 2) {
            uint8_t b0 = (d[i/8] >> (7-(i%8))) & 1;
            uint8_t b1 = (d[(i+1)/8] >> (7-((i+1)%8))) & 1;
            if(b0 != b1) { // valid Manchester
                if(b0 == 0) dec[di/8] |= (1 << (7-(di%8))); // 01→1
                di++;
            }
        }
        if(di >= 24) {
            // Decoded: [preamble...][channel:4][serial:20][event:8]
            // Find FFFE preamble or just show last meaningful bytes
            uint8_t ev = dec[3]; // event byte if we got enough
            const char* st = (ev & 0x80) ? "OPEN" : (ev & 0x04) ? "hb" :
                (ev & 0x40) ? "TAMP" : (ev & 0x08) ? "LOBAT" : "ok";
            snprintf(out, sz, "%s %02X", st, ev);
        } else {
            snprintf(out, sz, "Evt %ub", len * 8);
        }
    } else if(p == NRProtoNexusTH && len >= 5) {
        uint16_t raw = ((uint16_t)(d[1] & 0x0F) << 8) | d[2];
        int16_t temp = (raw > 2048) ? (int16_t)(raw - 4096) : (int16_t)raw;
        uint8_t humi = ((d[3] & 0x0F) << 4) | (d[4] >> 4);
        if(temp > -400 && temp < 600 && humi <= 100)
            snprintf(out, sz, "%d.%dC %d%%", temp / 10, (temp < 0 ? -temp : temp) % 10, humi);
        else
            snprintf(out, sz, "bad frame");
    } else
        snprintf(out, sz, "TE=%u %db", len > 0 ? d[0] : 0, len * 8);
}

static void nr_dev_label(NRDev* d) {
    if(d->name[0]) return;
    if(d->proto == NRProtoPT2262)
        snprintf(d->name, NR_MAX_NAME, "Remote %02X", (unsigned)(d->dev_id & 0xFF));
    else if(d->proto == NRProtoEV1527) {
        // Name by device type from first signal's cmd nibble
        uint8_t cmd = (d->sig_count > 0) ? (d->sigs[0].raw[2] & 0xF) : 0;
        const char* prefix = (cmd == 0xF || cmd == 0x8) ? "PIR" :
            (cmd == 0xE) ? "Door" : (cmd == 0x2 || cmd == 0x4 || cmd == 0x1) ? "Rmt" : "Sens";
        snprintf(d->name, NR_MAX_NAME, "%s %05lX", prefix, (unsigned long)(d->dev_id & 0xFFFFF));
    } else if(d->proto == NRProtoKeeloq)
        snprintf(d->name, NR_MAX_NAME, "Fob %07lX", (unsigned long)(d->dev_id & 0xFFFFFFF));
    else if(d->proto == NRProtoHoneywell)
        snprintf(d->name, NR_MAX_NAME, "Alarm System");
    else if(d->proto == NRProtoFSK)
        snprintf(d->name, NR_MAX_NAME, "FSK Sensor");
    else if(d->proto == NRProtoNexusTH)
        snprintf(d->name, NR_MAX_NAME, "Weather %02X", (unsigned)(d->dev_id & 0xFF));
    else
        snprintf(d->name, NR_MAX_NAME, "Dev TE=%u", d->te);
}

// ============== Device ops ==============

static int8_t nr_find_dev(NRApp* a, NRProto p, uint32_t id) {
    for(uint8_t i = 0; i < a->dev_count; i++)
        if(a->devs[i].proto == p && a->devs[i].dev_id == id) return i;
    return -1;
}

// Can this device be replayed? PT2262/EV1527 always, plus CAME-style 868 MHz
static bool nr_can_replay(NRDev* d) {
    if(nr_replayable[d->proto]) return true;
    // CAME 12-bit on 868 MHz: stored as BinRAW but has known fixed encoding
    if(d->freq == 868350000 && d->sig_count > 0 && d->sigs[0].bits == 12) return true;
    return false;
}

static int8_t nr_find_sig(NRDev* d, uint8_t* data, uint8_t len) {
    for(uint8_t i = 0; i < d->sig_count; i++)
        if(d->sigs[i].raw_len == len && memcmp(d->sigs[i].raw, data, len) == 0) return i;
    return -1;
}

static void nr_autosave_sig(NRApp* a, NRDev* d, NRSig* s) {
    if(!a->autosave) return;
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    storage_simply_mkdir(st, NR_AUTOSAVE_DIR);
    char path[80];
    // Save .txt with our decoded info + firmware decode string
    snprintf(path, sizeof(path), "%s/%s_%04d.txt",
        NR_AUTOSAVE_DIR, nr_pname[d->proto], a->autosave_seq);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, path)) {
        flipper_format_write_header_cstr(ff, "Neighborhood Signal", 1);
        flipper_format_write_string_cstr(ff, "Proto", nr_pname[d->proto]);
        flipper_format_write_string_cstr(ff, "Device", d->name);
        flipper_format_write_string_cstr(ff, "Signal", s->label);
        uint32_t v[2] = {d->te, s->bits};
        flipper_format_write_uint32(ff, "Info", v, 2);
        if(s->raw_len) flipper_format_write_hex(ff, "Data", s->raw, s->raw_len);
        // Append firmware protocol decode if available
        if(a->dec_proto[0]) {
            flipper_format_write_string_cstr(ff, "FW_Proto", a->dec_proto);
            flipper_format_write_string_cstr(ff, "FW_Decode", a->dec_str);
        }
    }
    flipper_format_free(ff);
    // Save .sub — BinRAW fallback (always, for replay/archive)
    snprintf(path, sizeof(path), "%s/%s_%04d.sub",
        NR_AUTOSAVE_DIR, nr_pname[d->proto], a->autosave_seq);
    File* file = storage_file_alloc(st);
    if(storage_file_open(file, path, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FuriString* line = furi_string_alloc();
        furi_string_printf(line,
            "Filetype: Flipper SubGhz Key File\n"
            "Version: 1\n"
            "Frequency: %lu\n"
            "Preset: FuriHalSubGhzPresetOok650Async\n"
            "Protocol: BinRAW\n"
            "Bit: %u\n"
            "TE: %u\n"
            "Bit_RAW: %u\nData_RAW:",
            (unsigned long)(d->freq ? d->freq : a->rx_freq), s->bits, d->te, s->bits);
        for(uint8_t i = 0; i < s->raw_len; i++)
            furi_string_cat_printf(line, " %02X", s->raw[i]);
        furi_string_cat(line, "\n");
        storage_file_write(file, furi_string_get_cstr(line), furi_string_size(line));
        furi_string_free(line);
    }
    storage_file_close(file);
    storage_file_free(file);
    a->autosave_seq++;
    furi_record_close(RECORD_STORAGE);
}

// ============== Save/Load DB ==============

static void nr_save(NRApp* a) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, NR_SAVE_FILE)) {
        flipper_format_write_header_cstr(ff, "Neighborhood DB", 5);
        uint32_t cnt = a->dev_count;
        flipper_format_write_uint32(ff, "Count", &cnt, 1);
        for(uint8_t i = 0; i < a->dev_count; i++) {
            NRDev* d = &a->devs[i];
            uint32_t h[6] = {d->proto, d->te, d->dev_id, d->hits, d->sig_count, d->seeded};
            flipper_format_write_uint32(ff, "Dev", h, 6);
            flipper_format_write_string_cstr(ff, "Name", d->name);
            flipper_format_write_string_cstr(ff, "Date",
                d->last_seen_date[0] ? d->last_seen_date : "--");
            uint32_t fq = d->freq;
            flipper_format_write_uint32(ff, "Freq", &fq, 1);
            for(uint8_t s = 0; s < d->sig_count; s++) {
                flipper_format_write_string_cstr(ff, "SL", d->sigs[s].label);
                uint32_t sb[2] = {d->sigs[s].bits, d->sigs[s].raw_len};
                flipper_format_write_uint32(ff, "SB", sb, 2);
                if(d->sigs[s].raw_len > 0)
                    flipper_format_write_hex(ff, "SD", d->sigs[s].raw, d->sigs[s].raw_len);
            }
        }
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

static void nr_load(NRApp* a) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    a->dev_count = 0;
    if(flipper_format_file_open_existing(ff, NR_SAVE_FILE)) {
        uint32_t ver = 0;
        FuriString* t = furi_string_alloc();
        if(flipper_format_read_header(ff, t, &ver) && (ver >= 3 && ver <= 5)) {
            uint32_t cnt = 0;
            flipper_format_read_uint32(ff, "Count", &cnt, 1);
            if(cnt > NR_MAX_DEVICES) cnt = NR_MAX_DEVICES;
            FuriString* s = furi_string_alloc();
            for(uint32_t i = 0; i < cnt; i++) {
                NRDev* d = &a->devs[i];
                memset(d, 0, sizeof(NRDev));
                uint32_t h[6] = {0};
                if(!flipper_format_read_uint32(ff, "Dev", h, 6)) break;
                d->proto = h[0] < NRProtoCount ? h[0] : NRProtoBinRAW;
                d->te = h[1]; d->dev_id = h[2]; d->hits = h[3];
                uint8_t sc = h[4]; if(sc > NR_MAX_SIGS) sc = NR_MAX_SIGS;
                d->seeded = h[5];
                if(flipper_format_read_string(ff, "Name", s))
                    snprintf(d->name, NR_MAX_NAME, "%s", furi_string_get_cstr(s));
                if(ver >= 4 && flipper_format_read_string(ff, "Date", s)) {
                    const char* ds = furi_string_get_cstr(s);
                    if(strcmp(ds, "--") != 0)
                        snprintf(d->last_seen_date, 12, "%s", ds);
                }
                if(ver >= 5) {
                    uint32_t fq = 0;
                    if(flipper_format_read_uint32(ff, "Freq", &fq, 1)) d->freq = fq;
                }
                if(!d->freq) d->freq = 433920000;
                for(uint8_t j = 0; j < sc; j++) {
                    if(!flipper_format_read_string(ff, "SL", s)) break;
                    snprintf(d->sigs[j].label, 20, "%s", furi_string_get_cstr(s));
                    uint32_t sb[2] = {0, 0};
                    if(!flipper_format_read_uint32(ff, "SB", sb, 2)) break;
                    d->sigs[j].bits = sb[0];
                    uint8_t rl = sb[1]; if(rl > 32) rl = 32;
                    if(rl > 0) {
                        uint8_t dd[32] = {0};
                        if(flipper_format_read_hex(ff, "SD", dd, rl)) {
                            memcpy(d->sigs[j].raw, dd, rl);
                            d->sigs[j].raw_len = rl;
                        }
                    }
                    d->sig_count++;
                }
                // Restore useful flag based on protocol (not saved in file)
                d->useful = (d->proto != NRProtoBinRAW);
                if(d->proto == NRProtoNexusTH && d->sig_count > 0 &&
                   strstr(d->sigs[0].label, "bad frame")) d->useful = false;
                a->dev_count++;
            }
            furi_string_free(s);
        }
        // If ver != 3, old format — ignore, seed will populate
        furi_string_free(t);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

static void nr_seed(NRApp* a) {
    if(a->dev_count > 0) return; // already have data
    #define SEED(P,TE,ID,HITS,NAME,DATE,FREQ) { \
        NRDev* d = &a->devs[a->dev_count++]; memset(d,0,sizeof(NRDev)); \
        d->proto=P; d->te=TE; d->dev_id=ID; d->hits=HITS; d->seeded=true; \
        d->useful=true; d->freq=FREQ; snprintf(d->name, NR_MAX_NAME, NAME); \
        snprintf(d->last_seen_date, 12, DATE); }

    SEED(NRProtoHoneywell, 143, 0x5800, 1633, "Alarm System", "May 2", 433920000);
    SEED(NRProtoKeeloq, 322, 0x2F9AE15, 24, "Parking Fob", "Apr 27", 433920000);
    a->devs[a->dev_count-1].sig_count = 2;
    snprintf(a->devs[a->dev_count-1].sigs[0].label, 20, "S2 2F9AE1");
    snprintf(a->devs[a->dev_count-1].sigs[1].label, 20, "S3 2F9AE1");

    SEED(NRProtoPT2262, 194, 0x4F, 53, "Remote 4F", "May 2", 433920000);
    NRDev* r = &a->devs[a->dev_count-1];
    r->sigs[0] = (NRSig){{0xFF,0xFE,0x4F,0xFF,0xE0},5,40,"Cmd:E0 (Btn A)"};
    r->sigs[1] = (NRSig){{0x00,0x44,0x80},3,24,"Cmd:22 (Btn B)"};
    r->sig_count = 2;

    SEED(NRProtoFSK, 65, 0xF5C0, 118, "FSK Sensor", "Apr 30", 433920000);
    SEED(NRProtoBinRAW, 98, 0xB109, 699, "OOK Unknown 98", "May 2", 433920000);
    SEED(NRProtoNexusTH, 650, 0xE0E0, 29, "Weather E0", "May 2", 433920000);
    a->devs[a->dev_count-1].sig_count = 1;
    snprintf(a->devs[a->dev_count-1].sigs[0].label, 20, "16.5C");
    SEED(NRProtoBinRAW, 345, 0xB122, 10, "Bell Ctrl", "May 2", 433920000);

    // 868 MHz devices
    SEED(NRProtoBinRAW, 320, 0x09EC, 19, "Garage", "May 4", 868350000);
    { NRDev* g = &a->devs[a->dev_count-1];
      g->sigs[0] = (NRSig){{0x9E,0xC0},2,12,"CAME 0x9EC"};
      g->sig_count = 1; }
    #undef SEED
}

// ============== RX / Radio / TX ==============

// Firmware protocol decode callback — fires from worker thread
static void nr_decode_cb(SubGhzReceiver* rx, SubGhzProtocolDecoderBase* db, void* ctx) {
    UNUSED(rx);
    NRApp* a = ctx;
    if(a->dec_ready) return; // drop if previous not processed
    strncpy(a->dec_proto, db->protocol->name, sizeof(a->dec_proto) - 1);
    FuriString* text = furi_string_alloc();
    subghz_protocol_decoder_base_get_string(db, text);
    strncpy(a->dec_str, furi_string_get_cstr(text), sizeof(a->dec_str) - 1);
    furi_string_free(text);
    a->dec_ready = true;
}

static void nr_rx_cb(void* ctx, bool level, uint32_t duration) {
    NRApp* a = ctx;
    // Feed firmware protocol decoders
    subghz_receiver_decode(a->receiver, level, duration);
    if(level) { a->rx_pulse = duration; return; }
    uint32_t h = a->rx_pulse, l = duration;
    if(l > 5000) {
        if(a->rx_bit_count >= 24 && a->rx_te_n > 0 && !a->rx_ready) {
            uint16_t te = a->rx_te_sum / a->rx_te_n;
            uint8_t bl = (a->rx_bit_count + 7) / 8; if(bl > 32) bl = 32;
            uint8_t tmp[32];
            memset(tmp, 0, 32);
            for(uint16_t i = 0; i < a->rx_bit_count && i < 256; i++)
                if(a->rx_bits[i]) tmp[i/8] |= (1 << (7-(i%8)));
            // Repeat validation: EV1527/PT2262 range requires 2 identical frames
            bool need_repeat = (te >= 105 && te <= 215 && a->rx_bit_count <= 80);
            if(need_repeat) {
                bool match = (bl == a->rx_last_len && bl > 0 &&
                    abs((int)te - (int)a->rx_last_te) < 20 &&
                    memcmp(tmp, a->rx_last, bl) == 0);
                memcpy(a->rx_last, tmp, bl);
                a->rx_last_len = bl;
                a->rx_last_te = te;
                if(!match) { // first frame — store and wait for repeat
                    a->rx_bit_count = 0; a->rx_te_sum = 0; a->rx_te_n = 0;
                    return;
                }
            }
            memcpy(a->rx_fdata, tmp, 32);
            a->rx_fte = te; a->rx_fbits = a->rx_bit_count; a->rx_flen = bl;
            a->rx_ready = true;
        }
        a->rx_bit_count = 0; a->rx_te_sum = 0; a->rx_te_n = 0;
        return;
    }
    if(h < 50 || l < 50) return;
    uint32_t sh = h < l ? h : l;
    if(sh < 800) { a->rx_te_sum += sh; a->rx_te_n++; }
    if(a->rx_bit_count < 128) {
        // Universal bit decision: compare pulse vs gap duration
        // Works for all PWM ratios (1:2 CAME, 1:3 PT2262, gap-based NexusTH)
        a->rx_bits[a->rx_bit_count++] = (h >= l) ? 1 : 0;
    }
}

static void nr_rx_start(NRApp* a) {
    if(a->rx_on) return;
    subghz_devices_idle(a->radio);
    subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
    uint32_t freq = (a->freq_mode == NRFreq868) ? 868350000 : 433920000;
    a->rx_freq = freq;
    subghz_devices_set_frequency(a->radio, freq);
    a->rx_bit_count = 0; a->rx_te_sum = 0; a->rx_te_n = 0; a->rx_ready = false;
    a->dec_ready = false;
    subghz_receiver_reset(a->receiver);
    subghz_worker_set_pair_callback(a->worker, (SubGhzWorkerPairCallback)nr_rx_cb);
    subghz_worker_set_context(a->worker, a);
    subghz_devices_start_async_rx(a->radio, subghz_worker_rx_callback, a->worker);
    subghz_worker_start(a->worker);
    a->rx_on = true;
}

static void nr_rx_stop(NRApp* a) {
    if(!a->rx_on) return;
    subghz_worker_stop(a->worker);
    subghz_devices_stop_async_rx(a->radio);
    subghz_devices_idle(a->radio);
    a->rx_on = false;
}

static void nr_tx(NRApp* a, NRDev* d, NRSig* s) {
    if(!s->raw_len) return;
    bool was = a->rx_on; if(was) nr_rx_stop(a);
    subghz_devices_idle(a->radio);
    subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(a->radio, d->freq ? d->freq : 433920000);
    uint16_t te = d->te;
    // Detect CAME-style encoding: TE ~320, 12 bits, on 868 MHz
    bool is_came = (d->freq == 868350000 && s->bits == 12 && te >= 280 && te <= 360);
    for(int r = 0; r < 6; r++) {
        subghz_devices_set_tx(a->radio);
        if(is_came) {
            // CAME: header = LOW 47*TE, then HIGH TE
            furi_hal_gpio_write(&gpio_cc1101_g0, false); furi_delay_us(te * 47);
            furi_hal_gpio_write(&gpio_cc1101_g0, true); furi_delay_us(te);
            for(uint16_t i = 0; i < s->bits; i++) {
                uint8_t b = (s->raw[i/8] >> (7-(i%8))) & 1;
                // CAME: 1=LOW long+HIGH short, 0=LOW short+HIGH long
                furi_hal_gpio_write(&gpio_cc1101_g0, false); furi_delay_us(b ? te*2 : te);
                furi_hal_gpio_write(&gpio_cc1101_g0, true); furi_delay_us(b ? te : te*2);
            }
        } else {
            // PT2262/EV1527: sync = HIGH TE, LOW 31*TE
            furi_hal_gpio_write(&gpio_cc1101_g0, true); furi_delay_us(te);
            furi_hal_gpio_write(&gpio_cc1101_g0, false); furi_delay_us(te * 31);
            uint16_t te3 = te * 3;
            for(uint16_t i = 0; i < s->bits && i < 256; i++) {
                uint8_t b = (s->raw[i/8] >> (7-(i%8))) & 1;
                furi_hal_gpio_write(&gpio_cc1101_g0, true); furi_delay_us(b ? te3 : te);
                furi_hal_gpio_write(&gpio_cc1101_g0, false); furi_delay_us(b ? te : te3);
            }
        }
        furi_hal_gpio_write(&gpio_cc1101_g0, false);
        subghz_devices_idle(a->radio); furi_delay_ms(8);
    }
    if(was) nr_rx_start(a);
}

// ============== CAME Scan ==============
static void nr_came_tx_code(NRApp* a, uint16_t code) {
    a->came_tx = true;
    bool was = a->rx_on; if(was) nr_rx_stop(a);
    subghz_devices_idle(a->radio);
    subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(a->radio, 868350000);
    uint16_t te = 320;
    for(int r = 0; r < 3; r++) {
        subghz_devices_set_tx(a->radio);
        // CAME header: LOW 47*TE, HIGH TE
        furi_hal_gpio_write(&gpio_cc1101_g0, false); furi_delay_us(te * 47);
        furi_hal_gpio_write(&gpio_cc1101_g0, true); furi_delay_us(te);
        for(int8_t i = 11; i >= 0; i--) {
            uint8_t b = (code >> i) & 1;
            furi_hal_gpio_write(&gpio_cc1101_g0, false); furi_delay_us(b ? te*2 : te);
            furi_hal_gpio_write(&gpio_cc1101_g0, true); furi_delay_us(b ? te : te*2);
        }
        furi_hal_gpio_write(&gpio_cc1101_g0, false);
        subghz_devices_idle(a->radio); furi_delay_ms(8);
    }
    if(was) nr_rx_start(a);
    a->came_tx = false;
}

// ============== Process frame ==============

static void nr_process(NRApp* a) {
    // Process firmware protocol decode (higher priority than raw accumulator)
    if(a->dec_ready) {
        // Extract key from CAME decode string: "Key:0x000009EC"
        if(strncmp(a->dec_proto, "CAME", 4) == 0) {
            char* kp = strstr(a->dec_str, "Key:0x");
            if(kp) {
                uint32_t key = strtoul(kp + 6, NULL, 16);
                uint8_t nbits = 12; // CAME 12-bit default
                char* bp = strstr(a->dec_str, "bit");
                if(bp && bp > a->dec_str) {
                    // parse "12bit" or "24bit" before "bit"
                    char* np = bp - 1;
                    while(np > a->dec_str && *(np-1) >= '0' && *(np-1) <= '9') np--;
                    nbits = atoi(np);
                    if(nbits == 0) nbits = 12;
                }
                // Build proper device entry from firmware decode
                uint8_t raw[4]; uint8_t raw_len;
                if(nbits <= 16) {
                    raw[0] = (key >> 8) & 0xFF; raw[1] = key & 0xFF; raw_len = 2;
                } else {
                    raw[0] = (key >> 16) & 0xFF; raw[1] = (key >> 8) & 0xFF;
                    raw[2] = key & 0xFF; raw_len = 3;
                }
                uint32_t did = key & 0xFFFF;
                int8_t di = nr_find_dev(a, NRProtoBinRAW, did);
                if(di < 0) di = nr_find_dev(a, NRProtoBinRAW, 0x09EC); // match garage seed
                if(di >= 0) {
                    NRDev* d = &a->devs[di];
                    if((a->tick - d->last_seen) >= NR_HIT_COOLDOWN) {
                        d->hits++; d->last_seen = a->tick;
                    }
                    if(d->seeded) d->confirmed = true;
                    // Update signal with correct decoded data
                    if(d->sig_count > 0) {
                        memcpy(d->sigs[0].raw, raw, raw_len);
                        d->sigs[0].raw_len = raw_len;
                        d->sigs[0].bits = nbits;
                        snprintf(d->sigs[0].label, 20, "CAME 0x%lX", (unsigned long)key);
                    }
                    NRSig tmp = {.raw_len = raw_len, .bits = nbits};
                    memcpy(tmp.raw, raw, raw_len);
                    snprintf(tmp.label, 20, "CAME 0x%lX", (unsigned long)key);
                    nr_autosave_sig(a, d, &tmp);
                }
                a->dec_ready = false; a->dec_proto[0] = 0; a->dec_str[0] = 0;
                return; // handled via firmware decode, skip raw processing
            }
        }
        // For other firmware decodes, just note it for autosave annotation (handled below)
    }

    if(!a->rx_ready) return;
    uint16_t te = a->rx_fte, bits = a->rx_fbits;
    uint8_t len = a->rx_flen, data[32];
    memcpy(data, a->rx_fdata, len);
    a->rx_ready = false;

    NRProto p = nr_classify(te, bits, data, len);

    // Read RSSI while signal is fresh
    int8_t rssi = -127;
    if(a->rx_on) rssi = (int8_t)subghz_devices_get_rssi(a->radio);

    // Protocol lock: ignore non-matching
    if(a->lock_proto >= 0 && p != (NRProto)a->lock_proto) return;

    uint32_t did = nr_dev_id(p, data, len, te);

    // Filter noise: EV1527 addr 0 or power-of-2 only
    if(p == NRProtoEV1527 && (did == 0 || (did & (did - 1)) == 0)) return;
    // Filter noise: PT2262 addr 0 only (0x02 is a real remote!)
    if(p == NRProtoPT2262 && did == 0) return;
    int8_t di = nr_find_dev(a, p, did);

    if(di >= 0) {
        NRDev* d = &a->devs[di];
        if((a->tick - d->last_seen) >= NR_HIT_COOLDOWN) {
            d->hits++; d->last_seen = a->tick; d->rssi = rssi;
        }
        if(d->seeded) d->confirmed = true;
        // NexusTH: update first signal label with latest temp reading (reject bad frames)
        if(p == NRProtoNexusTH && d->sig_count > 0 && len >= 5) {
            uint16_t raw = ((uint16_t)(data[1] & 0x0F) << 8) | data[2];
            int16_t temp = (raw > 2048) ? (int16_t)(raw - 4096) : (int16_t)raw;
            uint8_t humi = ((data[3] & 0x0F) << 4) | (data[4] >> 4);
            if(temp < -400 || temp > 600 || humi > 100) return; // reject garbage
            d->useful = true; // good frame promotes device to useful
            nr_sig_label(p, data, len, d->sigs[0].label, sizeof(d->sigs[0].label));
            memcpy(d->sigs[0].raw, data, len);
            d->sigs[0].raw_len = len;
            d->sigs[0].bits = bits;
            nr_autosave_sig(a, d, &d->sigs[0]);
            a->dec_ready = false; a->dec_proto[0] = 0;
            return;
        }
        // Keeloq/Honeywell: update sigs[0] with latest capture (rolling code, events)
        if((p == NRProtoKeeloq || p == NRProtoHoneywell) && d->sig_count > 0) {
            nr_sig_label(p, data, len, d->sigs[0].label, sizeof(d->sigs[0].label));
            memcpy(d->sigs[0].raw, data, len);
            d->sigs[0].raw_len = len;
            d->sigs[0].bits = bits;
            return;
        }
        // Store unique signals for replayable protocols (in-memory)
        if(nr_replayable[p] && nr_find_sig(d, data, len) < 0 && d->sig_count < NR_MAX_SIGS) {
            NRSig* s = &d->sigs[d->sig_count];
            memset(s, 0, sizeof(NRSig));
            s->raw_len = len; s->bits = bits;
            memcpy(s->raw, data, len);
            nr_sig_label(p, data, len, s->label, sizeof(s->label));
            d->sig_count++;
            notification_message(a->notif, &sequence_blink_green_10);
        }
        // Autosave all protocols except Honeywell (too spammy)
        if(p != NRProtoHoneywell) {
            NRSig tmp = {.raw_len = len, .bits = bits};
            memcpy(tmp.raw, data, len);
            nr_sig_label(p, data, len, tmp.label, sizeof(tmp.label));
            nr_autosave_sig(a, d, &tmp);
            a->dec_ready = false; a->dec_proto[0] = 0;
        }
        return;
    }

    // New device
    uint8_t slot = a->dev_count;
    if(slot >= NR_MAX_DEVICES) {
        slot = 0;
        for(uint8_t i = 1; i < NR_MAX_DEVICES; i++)
            if(!a->devs[i].seeded && a->devs[i].hits < a->devs[slot].hits) slot = i;
        if(a->devs[slot].seeded) return; // don't evict seeded
    }
    NRDev* d = &a->devs[slot];
    memset(d, 0, sizeof(NRDev));
    d->proto = p; d->te = te; d->dev_id = did; d->freq = a->rx_freq;
    d->hits = 1; d->last_seen = a->tick; d->rssi = rssi;
    NRSig* s = &d->sigs[0];
    s->raw_len = len; s->bits = bits;
    memcpy(s->raw, data, len);
    nr_sig_label(p, data, len, s->label, sizeof(s->label));
    d->sig_count = 1;
    d->useful = (p != NRProtoBinRAW);
    // NexusTH with bad frames (sanity filter rejected) are not useful
    if(p == NRProtoNexusTH && strstr(s->label, "bad frame")) d->useful = false;
    nr_dev_label(d);
    if(slot >= a->dev_count && a->dev_count < NR_MAX_DEVICES) a->dev_count++;
    nr_autosave_sig(a, d, s);
    a->dec_ready = false;
    a->dec_proto[0] = 0;
    a->dec_str[0] = 0;
    notification_message(a->notif, &sequence_blink_cyan_10);
}

// ============== Drawing ==============

// Format age as compact string: "*" <2s, "5s", "2m", "1h", "--"
static void nr_age_str(char* out, uint8_t sz, uint32_t tick, uint32_t last_seen) {
    if(last_seen == 0) { snprintf(out, sz, "--"); return; }
    uint32_t age = (tick - last_seen) / 20; // ticks to seconds (~50ms tick)
    if(age < 2) snprintf(out, sz, "*");
    else if(age < 60) snprintf(out, sz, "%us", (unsigned)age);
    else if(age < 3600) snprintf(out, sz, "%um", (unsigned)(age/60));
    else { unsigned h = age/3600; if(h > 99) h = 99; snprintf(out, sz, "%uh", h); }
}

// RSSI bars: 4 levels
static const char* nr_rssi_icon(int8_t rssi) {
    if(rssi > -60) return "||||";
    if(rssi > -75) return "|||.";
    if(rssi > -85) return "||..";
    if(rssi > -95) return "|...";
    return "....";
}

// Layout constants: screen is 128x64 (y: 0-63)
// Header: y=0-12 (title at y=10, line at y=12)
// Content: y=14-52 (4 rows, 10px each: y=14,24,34,44, text baseline +8)
// Footer: y=54 (line), y=63 (text baseline)
#define HDR_Y     10
#define HDR_LINE  12
#define ROW_START 14
#define ROW_H     10
#define MAX_ROWS  4
#define FTR_LINE  54
#define FTR_Y     63

static void nr_draw(Canvas* c, void* ctx) {
    NRApp* a = ctx;
    canvas_clear(c);
    char buf[48];

    if(a->view == NRViewMenu) {
        canvas_set_font(c, FontPrimary);
        canvas_draw_str(c, 20, HDR_Y, "NEIGHBORHOOD");
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);

        const char* items[] = {"Scan", "My Remotes", "Known Devices", "Sensors", "Settings", "CAME Scan"};
        uint8_t rc = 0;
        for(uint8_t i = 0; i < a->dev_count; i++)
            if(nr_can_replay(&a->devs[i])) rc++;
        uint8_t mstart = a->menu_sel > 3 ? a->menu_sel - 3 : 0;
        for(uint8_t i = mstart; i < 6; i++) {
            uint8_t y = ROW_START + (i - mstart) * ROW_H;
            if(y + ROW_H > FTR_LINE) break;
            if(i == a->menu_sel) {
                canvas_draw_box(c, 0, y, 128, ROW_H);
                canvas_set_color(c, ColorWhite);
            }
            canvas_draw_str(c, 6, y + 8, items[i]);
            if(i == 1 && rc > 0) {
                snprintf(buf, sizeof(buf), "(%d)", rc);
                canvas_draw_str(c, 90, y + 8, buf);
            } else if(i == 2) {
                uint8_t uc = 0;
                for(uint8_t j = 0; j < a->dev_count; j++)
                    if(a->devs[j].useful) uc++;
                snprintf(buf, sizeof(buf), "(%d)", uc);
                canvas_draw_str(c, 90, y + 8, buf);
            } else if(i == 3) {
                uint8_t sc = 0;
                for(uint8_t j = 0; j < a->dev_count; j++)
                    if(nr_is_sensor[a->devs[j].proto] && a->devs[j].useful) sc++;
                snprintf(buf, sizeof(buf), "(%d)", sc);
                canvas_draw_str(c, 90, y + 8, buf);
            }
            canvas_set_color(c, ColorBlack);
        }
        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "OK:Select  Bk:Exit");

    } else if(a->view == NRViewScan) {
        canvas_set_font(c, FontPrimary);
        const char* anim[] = {"~   "," ~  ","  ~ ","   ~","  ~ "," ~  "};
        if(a->lock_proto < 0)
            snprintf(buf, sizeof(buf), "SCAN %s", anim[a->scan_anim % 6]);
        else
            snprintf(buf, sizeof(buf), "SCAN [%s]", nr_pname[a->lock_proto]);
        canvas_draw_str(c, 0, HDR_Y, buf);
        uint8_t live = 0;
        for(uint8_t i = 0; i < a->dev_count; i++)
            if(a->devs[i].useful &&
               ((!a->devs[i].seeded && a->devs[i].last_seen >= a->session_start) ||
               (a->devs[i].seeded && a->devs[i].confirmed && a->devs[i].last_seen >= a->session_start))) live++;
        const char* fq = (a->rx_freq == 868350000) ? "868" : "433";
        snprintf(buf, sizeof(buf), "%s %d", fq, live);
        canvas_draw_str_aligned(c, 127, HDR_Y, AlignRight, AlignBottom, buf);
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);

        uint8_t vis = 0, row = 0;
        for(uint8_t i = 0; i < a->dev_count && row < MAX_ROWS; i++) {
            NRDev* d = &a->devs[i];
            bool is_live = (!d->seeded && d->last_seen >= a->session_start) ||
                           (d->seeded && d->confirmed && d->last_seen >= a->session_start);
            if(!is_live || !d->useful) continue;
            if(vis < a->sel) { vis++; continue; }
            uint8_t y = ROW_START + row * ROW_H;
            if(vis == a->sel) {
                canvas_draw_box(c, 0, y, 128, ROW_H);
                canvas_set_color(c, ColorWhite);
            }
            uint32_t age = a->tick - d->last_seen;
            char ac = age < 100 ? '*' : age < 400 ? '.' : ' ';
            if(d->proto == NRProtoNexusTH && d->sig_count > 0)
                snprintf(buf, sizeof(buf), "%c%s %s %s",
                    ac, nr_rssi_icon(d->rssi), d->sigs[0].label,
                    d->sig_count > 1 ? "+" : "");
            else
                snprintf(buf, sizeof(buf), "%c%s%s %s",
                    ac, nr_rssi_icon(d->rssi), nr_picon[d->proto], d->name);
            buf[42] = 0;
            canvas_draw_str(c, 0, y + 8, buf);
            canvas_set_color(c, ColorBlack);
            vis++; row++;
        }
        if(row == 0) canvas_draw_str(c, 10, 32, "Listening...");

        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "OK:Lock LongOK:Save");
        canvas_draw_str_aligned(c, 127, FTR_Y, AlignRight, AlignBottom, "Bk");

    } else if(a->view == NRViewRemotes) {
        canvas_set_font(c, FontPrimary);
        canvas_draw_str(c, 0, HDR_Y, "MY REMOTES");
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);

        uint8_t rc = 0, ri = 255;
        for(uint8_t i = 0; i < a->dev_count; i++) {
            if(!nr_can_replay(&a->devs[i])) continue;
            if(rc == a->sel) ri = i;
            rc++;
        }
        if(ri < a->dev_count) {
            NRDev* d = &a->devs[ri];
            snprintf(buf, sizeof(buf), "%s", d->name);
            canvas_draw_str_aligned(c, 64, ROW_START + 8, AlignCenter, AlignBottom, buf);
            if(rc > 1) {
                canvas_draw_str(c, 0, ROW_START + 8, "<");
                canvas_draw_str_aligned(c, 127, ROW_START + 8, AlignRight, AlignBottom, ">");
            }
            char age[6]; nr_age_str(age, sizeof(age), a->tick, d->last_seen);
            const char* when = (d->last_seen == 0 && d->last_seen_date[0]) ?
                d->last_seen_date : age;
            snprintf(buf, sizeof(buf), "%lux  %s", (unsigned long)d->hits, when);
            canvas_draw_str_aligned(c, 64, ROW_START + ROW_H + 7, AlignCenter, AlignBottom, buf);
            canvas_draw_line(c, 0, ROW_START + ROW_H + 8, 127, ROW_START + ROW_H + 8);
            for(uint8_t s = 0; s < d->sig_count; s++) {
                uint8_t y = ROW_START + ROW_H + 10 + s * ROW_H;
                if(y + ROW_H > FTR_LINE) break;
                if(s == a->dev_scroll) {
                    canvas_draw_box(c, 0, y, 128, ROW_H);
                    canvas_set_color(c, ColorWhite);
                }
                snprintf(buf, sizeof(buf), "  > %s", d->sigs[s].label);
                canvas_draw_str(c, 2, y + 8, buf);
                canvas_set_color(c, ColorBlack);
            }
        } else {
            canvas_draw_str(c, 4, 30, "No replayable devices");
        }
        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "OK:SEND  L/R:Dev  Bk");

        if(a->tx_flash && (a->tick - a->tx_flash) < 30) {
            canvas_draw_box(c, 34, 20, 60, 20);
            canvas_set_color(c, ColorWhite);
            canvas_set_font(c, FontPrimary);
            canvas_draw_str_aligned(c, 64, 34, AlignCenter, AlignBottom, "SENT!");
            canvas_set_color(c, ColorBlack);
        }

    } else if(a->view == NRViewKnown) {
        canvas_set_font(c, FontPrimary);
        canvas_draw_str(c, 0, HDR_Y, "KNOWN DEVICES");
        const char* ksc = a->rx_on ? ((a->tick / 5) % 2 ? "*" : "") : "";
        // Count useful devices
        uint8_t useful_count = 0;
        for(uint8_t i = 0; i < a->dev_count; i++)
            if(a->devs[i].useful) useful_count++;
        snprintf(buf, sizeof(buf), "%s %d", ksc, useful_count);
        canvas_draw_str_aligned(c, 127, HDR_Y, AlignRight, AlignBottom, buf);
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);

        // Build visible index → device index mapping
        uint8_t vis_idx[NR_MAX_DEVICES];
        uint8_t vis_count = 0;
        for(uint8_t i = 0; i < a->dev_count; i++)
            if(a->devs[i].useful) vis_idx[vis_count++] = i;

        if(a->sel >= vis_count && vis_count > 0) a->sel = vis_count - 1;
        uint8_t start = a->sel > 3 ? a->sel - 3 : 0;
        for(uint8_t vi = start; vi < vis_count && (vi - start) < MAX_ROWS; vi++) {
            NRDev* d = &a->devs[vis_idx[vi]];
            uint8_t y = ROW_START + (vi - start) * ROW_H;
            if(vi == a->sel) {
                canvas_draw_box(c, 0, y, 128, ROW_H);
                canvas_set_color(c, ColorWhite);
            }
            char tag = d->seeded ? (d->confirmed ? '+' : ' ') : '*';
            char age[6]; nr_age_str(age, sizeof(age), a->tick, d->last_seen);
            const char* when = (d->last_seen == 0 && d->last_seen_date[0]) ?
                d->last_seen_date : age;
            const char* dname = (d->proto == NRProtoNexusTH && d->sig_count > 0 && d->confirmed)
                ? d->sigs[0].label : d->name;
            snprintf(buf, sizeof(buf), "%c%s %s %lu %s",
                tag, nr_picon[d->proto], dname, (unsigned long)d->hits, when);
            buf[42] = 0;
            canvas_draw_str(c, 0, y + 8, buf);
            canvas_set_color(c, ColorBlack);
        }

        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "OK:Detail  Bk:Menu");

    } else if(a->view == NRViewDevice) {
        if(a->dev_sel >= a->dev_count) { a->view = NRViewKnown; return; }
        NRDev* d = &a->devs[a->dev_sel];
        canvas_set_font(c, FontPrimary);
        // Header: device name (or live temp for NexusTH)
        const char* hname = (d->proto == NRProtoNexusTH && d->sig_count > 0 && d->confirmed)
            ? d->sigs[0].label : d->name;
        snprintf(buf, sizeof(buf), "%s %s", nr_picon[d->proto], hname);
        canvas_draw_str(c, 0, HDR_Y, buf);
        // Right: scan indicator + RSSI or last seen date
        char age[6]; nr_age_str(age, sizeof(age), a->tick, d->last_seen);
        const char* blink = a->rx_on ? ((a->tick / 5) % 2 ? "*" : "") : "";
        if(d->last_seen == 0 && d->last_seen_date[0])
            snprintf(buf, sizeof(buf), "%s %s", d->last_seen_date, blink);
        else
            snprintf(buf, sizeof(buf), "%s %s %s", age, nr_rssi_icon(d->rssi), blink);
        canvas_draw_str_aligned(c, 127, HDR_Y, AlignRight, AlignBottom, buf);
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);

        int8_t line = -(int8_t)a->dev_scroll;
        // Protocol-specific info line
        if(d->proto == NRProtoNexusTH) {
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                if(d->sig_count > 0)
                    snprintf(buf, sizeof(buf), " >> %s", d->sigs[0].label);
                else
                    snprintf(buf, sizeof(buf), " Waiting for data...");
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                char age[6]; nr_age_str(age, sizeof(age), a->tick, d->last_seen);
                snprintf(buf, sizeof(buf), " %lu hits TE=%u %s ago",
                    (unsigned long)d->hits, d->te, age);
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
        } else if(d->proto == NRProtoHoneywell) {
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                snprintf(buf, sizeof(buf), " %lu hits TE=%u Manch",
                    (unsigned long)d->hits, d->te);
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                char age[6]; nr_age_str(age, sizeof(age), a->tick, d->last_seen);
                snprintf(buf, sizeof(buf), " %s %s ago",
                    d->confirmed ? "Active" : "Seeded", age);
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
        } else if(d->proto == NRProtoKeeloq) {
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                snprintf(buf, sizeof(buf), " %lu hits TE=%u Rolling",
                    (unsigned long)d->hits, d->te);
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
        } else if(d->proto == NRProtoEV1527) {
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                snprintf(buf, sizeof(buf), " %lu hits TE=%u Fixed",
                    (unsigned long)d->hits, d->te);
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
        } else if(d->proto != NRProtoNexusTH) {
            // FSK, BinRAW, PT2262
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                snprintf(buf, sizeof(buf), " %lu hits TE=%u",
                    (unsigned long)d->hits, d->te);
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
        }
        for(uint8_t s = 0; s < d->sig_count; s++) {
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t y = ROW_START + line * ROW_H;
                snprintf(buf, sizeof(buf), " %s %s",
                    nr_can_replay(d) ? ">" : " ", d->sigs[s].label);
                canvas_draw_str(c, 0, y + 8, buf);
            }
            line++;
        }
        if(line >= 0 && line < MAX_ROWS) {
            uint8_t y = ROW_START + line * ROW_H + 4;
            canvas_draw_line(c, 0, y, 127, y);
        }
        line++;
        const char* desc = nr_pdesc[d->proto];
        const char* p = desc;
        while(*p) {
            const char* nl = p; while(*nl && *nl != '\n') nl++;
            if(line >= 0 && line < MAX_ROWS) {
                uint8_t len = nl - p; if(len >= sizeof(buf)) len = sizeof(buf)-1;
                memcpy(buf, p, len); buf[len] = 0;
                canvas_draw_str(c, 2, ROW_START + line * ROW_H + 8, buf);
            }
            line++;
            p = *nl ? nl + 1 : nl;
        }
        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        if(nr_can_replay(d))
            canvas_draw_str(c, 0, FTR_Y, "OK:Send LOK:Lock");
        else
            canvas_draw_str(c, 0, FTR_Y, "LOK:Lock U/D:Scroll");
        canvas_draw_str_aligned(c, 127, FTR_Y, AlignRight, AlignBottom, "L/R Bk");

        if(a->tx_flash && (a->tick - a->tx_flash) < 30) {
            canvas_draw_box(c, 34, 20, 60, 20);
            canvas_set_color(c, ColorWhite);
            canvas_set_font(c, FontPrimary);
            canvas_draw_str_aligned(c, 64, 34, AlignCenter, AlignBottom, "SENT!");
            canvas_set_color(c, ColorBlack);
        }

    } else if(a->view == NRViewSensors) {
        canvas_set_font(c, FontPrimary);
        canvas_draw_str(c, 0, HDR_Y, "SENSORS");
        const char* blink = a->rx_on ? ((a->tick / 5) % 2 ? "*" : "") : "";
        canvas_draw_str_aligned(c, 127, HDR_Y, AlignRight, AlignBottom, blink);
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);

        uint8_t si[NR_MAX_DEVICES], sc = 0;
        for(uint8_t i = 0; i < a->dev_count; i++)
            if(nr_is_sensor[a->devs[i].proto] && a->devs[i].useful) si[sc++] = i;

        uint8_t start = a->sel > 3 ? a->sel - 3 : 0;
        for(uint8_t j = start; j < sc && (j - start) < MAX_ROWS; j++) {
            NRDev* d = &a->devs[si[j]];
            uint8_t y = ROW_START + (j - start) * ROW_H;
            if(j == a->sel) {
                canvas_draw_box(c, 0, y, 128, ROW_H);
                canvas_set_color(c, ColorWhite);
            }
            char age[6]; nr_age_str(age, sizeof(age), a->tick, d->last_seen);
            char tag = d->seeded ? (d->confirmed ? '+' : ' ') : '*';
            const char* when = (d->last_seen == 0 && d->last_seen_date[0]) ?
                d->last_seen_date : age;
            if(d->proto == NRProtoNexusTH && d->sig_count > 0 && d->confirmed)
                snprintf(buf, sizeof(buf), "%c~ %s %lu %s",
                    tag, d->sigs[0].label, (unsigned long)d->hits, when);
            else if(d->proto == NRProtoHoneywell)
                snprintf(buf, sizeof(buf), "%c# %s %lu %s",
                    tag, d->name, (unsigned long)d->hits, when);
            else
                snprintf(buf, sizeof(buf), "%c%s %s %lu %s",
                    tag, nr_picon[d->proto], d->name, (unsigned long)d->hits, when);
            buf[42] = 0;
            canvas_draw_str(c, 0, y + 8, buf);
            canvas_set_color(c, ColorBlack);
        }
        if(sc == 0) canvas_draw_str(c, 10, 32, "No sensors found");

        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "OK:Detail LOK:Lock Bk");

    } else if(a->view == NRViewSettings) {
        canvas_set_font(c, FontPrimary);
        canvas_draw_str(c, 0, HDR_Y, "SETTINGS");
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);

        for(uint8_t i = 0; i < 4; i++) {
            uint8_t y = ROW_START + i * ROW_H;
            if(a->sel == i) {
                canvas_draw_box(c, 0, y, 128, ROW_H);
                canvas_set_color(c, ColorWhite);
            }
            if(i == 0)
                snprintf(buf, sizeof(buf), "Freq: [%s MHz]", nr_freq_names[a->freq_mode]);
            else if(i == 1)
                snprintf(buf, sizeof(buf), "Protocol Lock: [%s]",
                    a->lock_proto < 0 ? "ALL" : nr_pname[a->lock_proto]);
            else if(i == 2)
                snprintf(buf, sizeof(buf), "Autosave: [%s]", a->autosave ? "ON" : "OFF");
            else
                snprintf(buf, sizeof(buf), "Clear Live Data");
            canvas_draw_str(c, 2, y + 8, buf);
            canvas_set_color(c, ColorBlack);
        }
        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "OK:Change  Bk:Menu");
    } else if(a->view == NRViewCameScan) {
        canvas_set_font(c, FontPrimary);
        canvas_draw_str(c, 0, HDR_Y, "CAME 868 SCAN");
        canvas_draw_line(c, 0, HDR_LINE, 127, HDR_LINE);
        canvas_set_font(c, FontSecondary);
        snprintf(buf, sizeof(buf), "Code: 0x%03X (%u/4096)", a->came_code, a->came_code + 1);
        canvas_draw_str(c, 2, 22, buf);
        snprintf(buf, sizeof(buf), "Progress: %lu%%", (unsigned long)a->came_code * 100 / 4096);
        canvas_draw_str(c, 2, 33, buf);
        if(a->came_tx)
            canvas_draw_str(c, 2, 44, ">>> TX <<<");
        else if(a->came_running)
            canvas_draw_str(c, 2, 44, "Scanning... OK:Pause");
        else
            canvas_draw_str(c, 2, 44, "OK:Send  Hold OK:Auto");
        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "U/D:Step  Bk:Exit");
    }
}

static void nr_input(InputEvent* e, void* ctx) {
    NRApp* a = ctx;
    furi_message_queue_put(a->eq, e, FuriWaitForever);
}

// ============== Helpers ==============

// Count live devices in scan
static uint8_t nr_live_count(NRApp* a) {
    uint8_t c = 0;
    for(uint8_t i = 0; i < a->dev_count; i++) {
        NRDev* d = &a->devs[i];
        if((!d->seeded && d->last_seen >= a->session_start) ||
           (d->seeded && d->confirmed && d->last_seen >= a->session_start)) c++;
    }
    return c;
}

// Get nth live device index
static int8_t nr_live_idx(NRApp* a, uint8_t n) {
    uint8_t c = 0;
    for(uint8_t i = 0; i < a->dev_count; i++) {
        NRDev* d = &a->devs[i];
        if((!d->seeded && d->last_seen >= a->session_start) ||
           (d->seeded && d->confirmed && d->last_seen >= a->session_start)) {
            if(c == n) return i;
            c++;
        }
    }
    return -1;
}

// ============== Main ==============

int32_t neighborhood_remote_app(void* p) {
    UNUSED(p);
    NRApp* a = malloc(sizeof(NRApp));
    memset(a, 0, sizeof(NRApp));
    a->lock_proto = -1;
    a->autosave = true;

    a->eq = furi_message_queue_alloc(8, sizeof(InputEvent));
    a->vp = view_port_alloc();
    view_port_draw_callback_set(a->vp, nr_draw, a);
    view_port_input_callback_set(a->vp, nr_input, a);
    a->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(a->gui, a->vp, GuiLayerFullscreen);
    a->notif = furi_record_open(RECORD_NOTIFICATION);

    subghz_devices_init();
    a->radio = subghz_devices_get_by_name(SUBGHZ_DEVICE_CC1101_INT_NAME);
    subghz_devices_begin(a->radio);
    subghz_devices_reset(a->radio);
    a->worker = subghz_worker_alloc();

    // Firmware protocol decoder chain
    a->environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(a->environment, (void*)&subghz_protocol_registry);
    a->receiver = subghz_receiver_alloc_init(a->environment);
    subghz_receiver_set_filter(a->receiver, SubGhzProtocolFlag_Decodable);
    subghz_receiver_set_rx_callback(a->receiver, nr_decode_cb, a);

    nr_load(a);
    nr_seed(a);
    a->view = NRViewMenu;
    a->session_start = 1; // tick starts at 0, session_start=1 means nothing is "live" yet

    bool running = true;
    InputEvent ev;
    while(running) {
        if(furi_message_queue_get(a->eq, &ev, 50) == FuriStatusOk) {
            if(ev.type != InputTypeShort && ev.type != InputTypeLong && ev.type != InputTypeRepeat) goto tick;

            if(a->view == NRViewMenu) {
                if(ev.key == InputKeyBack) {
                    running = false;
                } else if(ev.key == InputKeyUp && a->menu_sel > 0) {
                    a->menu_sel--;
                } else if(ev.key == InputKeyDown && a->menu_sel < 5) {
                    a->menu_sel++;
                } else if(ev.key == InputKeyOk) {
                    a->sel = 0;
                    if(a->menu_sel == 0) {
                        a->view = NRViewScan;
                        a->session_start = a->tick;
                        nr_rx_start(a);
                    } else if(a->menu_sel == 1) {
                        a->view = NRViewRemotes;
                    } else if(a->menu_sel == 2) {
                        a->view = NRViewKnown;
                        nr_rx_start(a);
                    } else if(a->menu_sel == 3) {
                        a->view = NRViewSensors;
                        a->sel = 0;
                        nr_rx_start(a);
                    } else if(a->menu_sel == 4) {
                        a->view = NRViewSettings;
                    } else if(a->menu_sel == 5) {
                        a->view = NRViewCameScan;
                    }
                }

            } else if(a->view == NRViewScan) {
                uint8_t lc = nr_live_count(a);
                if(ev.key == InputKeyBack) {
                    nr_rx_stop(a);
                    a->view = NRViewMenu;
                } else if(ev.key == InputKeyUp && a->sel > 0) {
                    a->sel--;
                } else if(ev.key == InputKeyDown && a->sel + 1 < lc) {
                    a->sel++;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeShort && lc > 0) {
                    // Short OK: lock on selected device's protocol
                    int8_t ri = nr_live_idx(a, a->sel);
                    if(ri >= 0) {
                        NRProto sp = a->devs[ri].proto;
                        if(a->lock_proto == (int8_t)sp)
                            a->lock_proto = -1; // toggle off
                        else
                            a->lock_proto = sp;
                    }
                } else if(ev.key == InputKeyOk && ev.type == InputTypeLong && lc > 0) {
                    // Long OK: save selected device to known + clear from live
                    int8_t ri = nr_live_idx(a, a->sel);
                    if(ri >= 0) {
                        // Save: reset hits so it drops to bottom, but keeps capturing
                        a->devs[ri].hits = 0;
                        notification_message(a->notif, &sequence_blink_green_100);
                        if(a->sel > 0) a->sel--;
                    }
                } else if(ev.key == InputKeyLeft || ev.key == InputKeyRight) {
                    // L/R: cycle frequency mode
                    if(ev.key == InputKeyRight) {
                        a->freq_mode = (a->freq_mode + 1) % 3;
                    } else {
                        a->freq_mode = a->freq_mode == 0 ? 2 : a->freq_mode - 1;
                    }
                    nr_rx_stop(a);
                    nr_rx_start(a);
                }

            } else if(a->view == NRViewRemotes) {
                // Count replayable devices
                uint8_t rc = 0;
                for(uint8_t i = 0; i < a->dev_count; i++)
                    if(nr_can_replay(&a->devs[i])) rc++;
                // Find current device
                uint8_t ri = 255, c2 = 0;
                for(uint8_t i = 0; i < a->dev_count; i++) {
                    if(!nr_can_replay(&a->devs[i])) continue;
                    if(c2 == a->sel) { ri = i; break; }
                    c2++;
                }

                if(ev.key == InputKeyBack) {
                    a->view = NRViewMenu;
                } else if(ev.key == InputKeyLeft && a->sel > 0) {
                    a->sel--; a->dev_scroll = 0;
                } else if(ev.key == InputKeyRight && a->sel + 1 < rc) {
                    a->sel++; a->dev_scroll = 0;
                } else if(ev.key == InputKeyUp && a->dev_scroll > 0) {
                    a->dev_scroll--;
                } else if(ev.key == InputKeyDown && ri < a->dev_count &&
                          a->dev_scroll + 1 < a->devs[ri].sig_count) {
                    a->dev_scroll++;
                } else if(ev.key == InputKeyOk && ri < a->dev_count) {
                    NRDev* d = &a->devs[ri];
                    uint8_t si = a->dev_scroll < d->sig_count ? a->dev_scroll : 0;
                    notification_message(a->notif, &sequence_blink_magenta_100);
                    nr_tx(a, d, &d->sigs[si]);
                    notification_message(a->notif, &sequence_blink_green_100);
                    a->tx_flash = a->tick;
                }

            } else if(a->view == NRViewKnown) {
                if(ev.key == InputKeyBack) {
                    nr_rx_stop(a);
                    a->view = NRViewMenu;
                } else if(ev.key == InputKeyUp && a->sel > 0) {
                    a->sel--;
                } else {
                    // Count useful devices for bounds
                    uint8_t uc = 0;
                    uint8_t ui[NR_MAX_DEVICES];
                    for(uint8_t i = 0; i < a->dev_count; i++)
                        if(a->devs[i].useful) ui[uc++] = i;
                    if(ev.key == InputKeyDown && a->sel + 1 < uc) {
                        a->sel++;
                    } else if(ev.key == InputKeyOk && a->sel < uc) {
                        a->dev_sel = ui[a->sel];
                        a->dev_scroll = 0;
                        a->view = NRViewDevice;
                        a->lock_proto = a->devs[ui[a->sel]].proto;
                        nr_rx_start(a);
                    }
                }

            } else if(a->view == NRViewDevice) {
                NRDev* d = a->dev_sel < a->dev_count ? &a->devs[a->dev_sel] : NULL;
                if(ev.key == InputKeyBack) {
                    a->lock_proto = -1; // clear lock on exit
                    nr_rx_stop(a);
                    a->view = NRViewKnown;
                    a->sel = a->dev_sel;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeShort &&
                          d && d->sig_count > 0 && nr_can_replay(d)) {
                    uint8_t si = a->dev_scroll < d->sig_count ? a->dev_scroll : 0;
                    notification_message(a->notif, &sequence_blink_magenta_100);
                    nr_tx(a, d, &d->sigs[si]);
                    notification_message(a->notif, &sequence_blink_green_100);
                    a->tx_flash = a->tick;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeLong && d) {
                    // Long OK: toggle protocol lock
                    if(a->lock_proto == (int8_t)d->proto)
                        a->lock_proto = -1; // unlock
                    else
                        a->lock_proto = d->proto; // re-lock
                } else if(ev.key == InputKeyUp && a->dev_scroll > 0) {
                    a->dev_scroll--;
                } else if(ev.key == InputKeyDown) {
                    if(a->dev_scroll < 20) a->dev_scroll++;
                } else if(ev.key == InputKeyLeft && a->dev_sel > 0) {
                    a->dev_sel--; a->dev_scroll = 0;
                    a->lock_proto = a->devs[a->dev_sel].proto; // update lock
                } else if(ev.key == InputKeyRight && a->dev_sel + 1 < a->dev_count) {
                    a->dev_sel++; a->dev_scroll = 0;
                    a->lock_proto = a->devs[a->dev_sel].proto; // update lock
                }

            } else if(a->view == NRViewSensors) {
                // Count sensors
                uint8_t si[NR_MAX_DEVICES], sc = 0;
                for(uint8_t i = 0; i < a->dev_count; i++)
                    if(nr_is_sensor[a->devs[i].proto]) si[sc++] = i;

                if(ev.key == InputKeyBack) {
                    a->lock_proto = -1;
                    nr_rx_stop(a);
                    a->view = NRViewMenu;
                } else if(ev.key == InputKeyUp && a->sel > 0) {
                    a->sel--;
                } else if(ev.key == InputKeyDown && a->sel + 1 < sc) {
                    a->sel++;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeShort && a->sel < sc) {
                    a->dev_sel = si[a->sel];
                    a->dev_scroll = 0;
                    a->view = NRViewDevice;
                    a->lock_proto = a->devs[si[a->sel]].proto;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeLong && a->sel < sc) {
                    NRProto sp = a->devs[si[a->sel]].proto;
                    if(a->lock_proto == (int8_t)sp)
                        a->lock_proto = -1;
                    else
                        a->lock_proto = sp;
                }

            } else if(a->view == NRViewSettings) {
                if(ev.key == InputKeyBack) {
                    a->view = NRViewMenu;
                } else if(ev.key == InputKeyUp && a->sel > 0) {
                    a->sel--;
                } else if(ev.key == InputKeyDown && a->sel < 3) {
                    a->sel++;
                } else if(ev.key == InputKeyOk) {
                    if(a->sel == 0) {
                        // Cycle frequency mode
                        a->freq_mode = (a->freq_mode + 1) % 3;
                        if(a->rx_on) {
                            nr_rx_stop(a);
                            nr_rx_start(a);
                        }
                    } else if(a->sel == 1) {
                        // Cycle protocol lock
                        a->lock_proto++;
                        if(a->lock_proto >= (int8_t)NRProtoCount) a->lock_proto = -1;
                    } else if(a->sel == 2) {
                        a->autosave = !a->autosave;
                    } else if(a->sel == 3 && ev.type == InputTypeLong) {
                        // Clear live data (keep seeded)
                        uint8_t w = 0;
                        for(uint8_t i = 0; i < a->dev_count; i++) {
                            if(a->devs[i].seeded) {
                                a->devs[i].confirmed = false;
                                if(w != i) a->devs[w] = a->devs[i];
                                w++;
                            }
                        }
                        a->dev_count = w;
                        notification_message(a->notif, &sequence_blink_yellow_100);
                    }
                }
            } else if(a->view == NRViewCameScan) {
                if(ev.key == InputKeyBack) {
                    a->came_running = false;
                    a->view = NRViewMenu;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeLong) {
                    a->came_running = !a->came_running;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeShort) {
                    if(a->came_running) {
                        a->came_running = false;
                    } else {
                        nr_came_tx_code(a, a->came_code);
                        view_port_update(a->vp);
                    }
                } else if(!a->came_running) {
                    if(ev.key == InputKeyUp) {
                        uint16_t step = (ev.type == InputTypeLong) ? 100 :
                                        (ev.type == InputTypeRepeat) ? 10 : 1;
                        a->came_code = (a->came_code + step) & 0xFFF;
                    } else if(ev.key == InputKeyDown) {
                        uint16_t step = (ev.type == InputTypeLong) ? 100 :
                                        (ev.type == InputTypeRepeat) ? 10 : 1;
                        a->came_code = (a->came_code - step) & 0xFFF;
                    }
                }
            }
        }

tick:
        a->tick++;
        // Auto freq mode: alternate 433/868 every 10 ticks (500ms)
        if(a->freq_mode == NRFreqAuto && a->rx_on && (a->tick - a->auto_switch) >= 10) {
            a->auto_switch = a->tick;
            uint32_t next = (a->rx_freq == 433920000) ? 868350000 : 433920000;
            nr_rx_stop(a);
            a->rx_freq = next;
            // rx_start uses freq_mode, override for auto
            subghz_devices_idle(a->radio);
            subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
            subghz_devices_set_frequency(a->radio, next);
            a->rx_bit_count = 0; a->rx_te_sum = 0; a->rx_te_n = 0; a->rx_ready = false;
            a->dec_ready = false;
            subghz_receiver_reset(a->receiver);
            subghz_worker_set_pair_callback(a->worker, (SubGhzWorkerPairCallback)nr_rx_cb);
            subghz_worker_set_context(a->worker, a);
            subghz_devices_start_async_rx(a->radio, subghz_worker_rx_callback, a->worker);
            subghz_worker_start(a->worker);
            a->rx_on = true;
        }
        // CAME auto-scan: TX current code and advance
        if(a->view == NRViewCameScan && a->came_running && !a->came_tx) {
            nr_came_tx_code(a, a->came_code);
            a->came_code = (a->came_code + 1) & 0xFFF;
            if(a->came_code == 0) a->came_running = false; // wrapped = done
            view_port_update(a->vp);
        }
        nr_process(a);
        if(a->view == NRViewScan && (a->tick % 8) == 0) a->scan_anim++;
        view_port_update(a->vp);
    }

    nr_save(a);
    nr_rx_stop(a);
    subghz_worker_free(a->worker);
    subghz_receiver_free(a->receiver);
    subghz_environment_free(a->environment);
    subghz_devices_end(a->radio);
    subghz_devices_deinit();
    gui_remove_view_port(a->gui, a->vp);
    view_port_free(a->vp);
    furi_message_queue_free(a->eq);
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);
    free(a);
    return 0;
}
