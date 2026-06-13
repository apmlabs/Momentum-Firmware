/*
 * Neighborhood v4 — 433.92 MHz device awareness & replay
 * Menu-first: Scan, My Remotes, Known Devices, Settings
 */
#include "neighborhood_remote.h"
#include <furi_hal_rtc.h>
#define TAG "Neighborhood"

static void nr_update_date(NRDev* d);

// ============== Classification ==============

static NRProto nr_classify(uint16_t te, uint16_t bits, uint8_t* d, uint8_t len) {
    if(len >= 3) {
        uint8_t ff = 0;
        for(uint8_t i = 0; i < len; i++) if(d[i] == 0xFF) ff++;
        if(ff > len / 3 && te < 70) return NRProtoFSK;
    }
    // Dooya A-OK: starts with 0xA3, TE 250-400, 64 bits — check before KeeLoq
    if(te >= 250 && te <= 400 && bits >= 60 && bits <= 70 && len >= 1 && d[0] >= 0xA0)
        return NRProtoBinRAW; // Dooya handled by custom decoder, classify as BinRAW
    // NexusTH handled by dedicated state machine (nr_nexus_decode), not bit accumulator
    if(te >= 500 && te <= 750) return NRProtoBinRAW;
    // KeeLoq: TE 220-400, 60-90 bits (single frame + preamble)
    if(te >= 220 && te <= 400 && bits >= 60 && bits <= 90) return NRProtoKeeloq;
    if(te >= 100 && te <= 210 && bits >= 30) return NRProtoHoneywell;
    if(te >= 70 && te <= 90 && bits >= 30) return NRProtoHoneywell; // half-bit Manchester
    // Princeton/PT2262: TE 175-400, 16-50 bits (covers Remote C6 at TE=380)
    if(te >= 175 && te <= 400 && bits >= 16 && bits <= 56) return NRProtoPT2262;
    if(te >= 105 && te <= 130 && bits >= 20 && bits <= 80) return NRProtoEV1527;
    return NRProtoBinRAW;
}

static uint32_t nr_dev_id(NRProto p, uint8_t* d, uint8_t len, uint16_t te) {
    if(p == NRProtoPT2262 && len >= 3) return d[0];
    if(p == NRProtoEV1527 && len >= 3)
        return (((uint32_t)d[0]<<16)|((uint32_t)d[1]<<8)|d[2]) >> 4;
    if(p == NRProtoKeeloq && len >= 8) {
        // For multi-frame captures, use last frame's serial (bytes at end)
        uint8_t off = (len > 9) ? len - 9 : 0;
        uint32_t sn = (((uint32_t)d[off+4]&0xF)<<24)|((uint32_t)d[off+5]<<16)|
                      ((uint32_t)d[off+6]<<8)|d[off+7];
        return sn >> 4;
    }
    if(p == NRProtoHoneywell) return 0x5800;
    if(p == NRProtoFSK) return 0xF5C0;
    if(p == NRProtoNexusTH && len >= 1) return 0xE000 | d[0];
    // Group OOK meter variants into single device
    if(p == NRProtoBinRAW && te >= 90 && te <= 109) return 0xB109;
    // TE 400-750 BinRAW is also OOK meter noise (TE estimation varies wildly)
    if(p == NRProtoBinRAW && te >= 400 && te <= 750) return 0xB109;
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

// Can this device be replayed? PT2262/EV1527 always, plus CAME-style 868 MHz, plus has_file
static bool nr_can_replay(NRDev* d) {
    if(nr_replayable[d->proto] && d->sig_count > 0) return true;
    if(d->freq == 868350000 && d->sig_count > 0 && d->sigs[0].bits == 12) return true;
    for(uint8_t i = 0; i < d->sig_count; i++)
        if(d->sigs[i].tx_key || d->sigs[i].has_file) return true;
    return false;
}

static int8_t nr_find_sig(NRDev* d, uint8_t* data, uint8_t len) {
    for(uint8_t i = 0; i < d->sig_count; i++)
        if(d->sigs[i].raw_len == len && memcmp(d->sigs[i].raw, data, len) == 0) return i;
    return -1;
}

static void nr_autosave_sig(NRApp* a, NRDev* d, NRSig* s) {
    if(!a->autosave) return;
    // Skip phantom KeeLoq (Interlogix alarm frames misclassified, always unique serial)
    if(d->proto == NRProtoKeeloq && !d->seeded && d->hits < 2) return;
    // Skip OOK meter and Honeywell half-TE noise (bulk of traffic, all identical)
    if(d->dev_id == 0xB109 || d->dev_id == 0xB108) return;
    // Skip high-entropy BinRAW in TE 200-400 range (encrypted alarm scatter)
    if(d->proto == NRProtoBinRAW && !d->seeded && d->te >= 200 && d->te <= 400 && d->hits < 3) return;
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
        int32_t rssi32 = d->rssi;
        flipper_format_write_int32(ff, "RSSI", &rssi32, 1);
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
        flipper_format_write_header_cstr(ff, "Neighborhood DB", 6);
        // Count persistable devices: seeded OR (hits >= 2 AND not KeeLoq)
        uint32_t cnt = 0;
        for(uint8_t i = 0; i < a->dev_count; i++) {
            NRDev* d = &a->devs[i];
            if(d->seeded || (d->hits >= 2 && d->proto != NRProtoKeeloq)) cnt++;
        }
        flipper_format_write_uint32(ff, "Count", &cnt, 1);
        for(uint8_t i = 0; i < a->dev_count; i++) {
            NRDev* d = &a->devs[i];
            if(!d->seeded && (d->hits < 2 || d->proto == NRProtoKeeloq)) continue;
            uint32_t h[6] = {d->proto, d->te, d->dev_id, d->hits, d->sig_count, d->seeded};
            flipper_format_write_uint32(ff, "Dev", h, 6);
            flipper_format_write_string_cstr(ff, "Name", d->name);
            flipper_format_write_string_cstr(ff, "Date",
                d->last_seen_date[0] ? d->last_seen_date : "--");
            uint32_t fq = d->freq;
            flipper_format_write_uint32(ff, "Freq", &fq, 1);
            int32_t rssi32 = d->rssi;
            flipper_format_write_int32(ff, "RSSI", &rssi32, 1);
            flipper_format_write_string_cstr(ff, "FWProto",
                d->fw_proto[0] ? d->fw_proto : "--");
            for(uint8_t s = 0; s < d->sig_count; s++) {
                flipper_format_write_string_cstr(ff, "SL", d->sigs[s].label);
                uint32_t sb[2] = {d->sigs[s].bits, d->sigs[s].raw_len};
                flipper_format_write_uint32(ff, "SB", sb, 2);
                if(d->sigs[s].raw_len > 0)
                    flipper_format_write_hex(ff, "SD", d->sigs[s].raw, d->sigs[s].raw_len);
                uint32_t sf[2] = {d->sigs[s].file_seq, d->sigs[s].has_file ? 1 : 0};
                flipper_format_write_uint32(ff, "SF", sf, 2);
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
        if(flipper_format_read_header(ff, t, &ver) && (ver >= 3 && ver <= 6)) {
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
                if(ver >= 6) {
                    int32_t rssi32 = -127;
                    if(flipper_format_read_int32(ff, "RSSI", &rssi32, 1)) d->rssi = (int8_t)rssi32;
                    if(flipper_format_read_string(ff, "FWProto", s)) {
                        const char* fp = furi_string_get_cstr(s);
                        if(strcmp(fp, "--") != 0) snprintf(d->fw_proto, 16, "%s", fp);
                    }
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
                    if(ver >= 6) {
                        uint32_t sf[2] = {0, 0};
                        if(flipper_format_read_uint32(ff, "SF", sf, 2)) {
                            d->sigs[j].file_seq = sf[0];
                            d->sigs[j].has_file = (sf[1] != 0);
                        }
                    }
                    d->sig_count++;
                }
                // Restore useful flag based on protocol (not saved in file)
                d->useful = (d->proto != NRProtoBinRAW) || d->seeded ||
                    (d->sig_count > 0 && d->sigs[0].has_file) ||
                    (d->proto == NRProtoBinRAW && d->hits >= 5);
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

    // Re-apply tx_keys (not saved in file) for known devices
    // dev_id must match what SEED macro stores (truncated address byte for Princeton)
    struct { uint32_t dev_id; uint64_t keys[4]; } known_tx[] = {
        {0x4F, {0xFFFE4F, 0x004480, 0, 0}},              // Remote 4F (Btn A, Btn B)
        {0x9C, {0x9CB871, 0x9CB872, 0x9CB874, 0x9CB878}},  // Neighbor Gate
        {0xC6, {0xC62C86, 0, 0, 0}},                   // Remote C6
        {0xEA, {0xEA55B1, 0, 0, 0}},                   // Remote EA
        {0x11, {0x11B172, 0, 0, 0}},                    // Remote 11
        {0x75, {0x75140B, 0, 0, 0}},                    // Gate
        {0xF7, {0xF72C9E, 0, 0, 0}},                    // Gate 2
        {0xC0A16C, {0xA3C0A16C01000BD9ULL, 0xA3C0A16C010023F1ULL, 0xA3C0A16C01004311ULL, 0}},
        {0xC0AD01, {0xA3C0AD0101000B7AULL, 0xA3C0AD01010023B2ULL, 0xA3C0AD01010043B2ULL, 0}},
        {0xC09EBD, {0xA3C09EBD01000B27ULL, 0xA3C09EBD0100233FULL, 0xA3C09EBD0100435FULL, 0}},
        {0x635A4B, {0xA3635A4B01000B14ULL, 0xA3635A4B0100232CULL, 0xA3635A4B0100434CULL, 0}},
        {0x09EC, {0x09EC, 0, 0, 0}},                    // Garage CAME
        {0x0100, {0x0100C0013FULL, 0x01001001EFULL, 0x01005001AFULL, 0}},  // Markisol
        {0x47864, {0x08F0C8F19ULL, 0x08F0C9503ULL, 0x08F0C891FULL, 0x08F0C9701ULL}},  // UniFan
    };
    for(uint8_t i = 0; i < a->dev_count; i++) {
        NRDev* d = &a->devs[i];
        for(size_t k = 0; k < sizeof(known_tx)/sizeof(known_tx[0]); k++) {
            if(d->dev_id == known_tx[k].dev_id) {
                for(uint8_t s = 0; s < d->sig_count && s < 4; s++) {
                    if(known_tx[k].keys[s]) d->sigs[s].tx_key = known_tx[k].keys[s];
                }
                break;
            }
        }
    }
}

// Generate a protocol .sub file for seeding (Princeton, CAME, etc.)
static void nr_seed_sub(NRApp* a, const char* proto, uint32_t freq, uint16_t bits,
    const char* key_hex, uint16_t te, uint16_t seq) {
    UNUSED(a);
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    storage_simply_mkdir(st, NR_AUTOSAVE_DIR);
    char path[80];
    snprintf(path, sizeof(path), "%s/%04d.sub", NR_AUTOSAVE_DIR, seq);
    if(storage_file_exists(st, path)) { furi_record_close(RECORD_STORAGE); return; }
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, path)) {
        flipper_format_write_header_cstr(ff, "Flipper SubGhz Key File", 1);
        flipper_format_write_uint32(ff, "Frequency", &freq, 1);
        flipper_format_write_string_cstr(ff, "Preset", "FuriHalSubGhzPresetOok650Async");
        flipper_format_write_string_cstr(ff, "Protocol", proto);
        uint32_t b32 = bits;
        flipper_format_write_uint32(ff, "Bit", &b32, 1);
        flipper_format_write_string_cstr(ff, "Key", key_hex);
        if(te > 0) { uint32_t t32 = te; flipper_format_write_uint32(ff, "TE", &t32, 1); }
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

// Write a RAW .sub file with pre-computed pulse timing
static void nr_seed_raw_sub(NRApp* a, uint32_t freq, const int16_t* pulses, uint16_t count, uint16_t seq) {
    UNUSED(a);
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    storage_simply_mkdir(st, NR_AUTOSAVE_DIR);
    char path[80];
    snprintf(path, sizeof(path), "%s/%04d.sub", NR_AUTOSAVE_DIR, seq);
    if(storage_file_exists(st, path)) { furi_record_close(RECORD_STORAGE); return; }
    File* file = storage_file_alloc(st);
    if(storage_file_open(file, path, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FuriString* s = furi_string_alloc();
        furi_string_printf(s,
            "Filetype: Flipper SubGhz RAW File\nVersion: 1\n"
            "Frequency: %lu\nPreset: FuriHalSubGhzPresetOok650Async\n"
            "Protocol: RAW\nRAW_Data:",
            (unsigned long)freq);
        for(uint16_t i = 0; i < count; i++)
            furi_string_cat_printf(s, " %d", (int)pulses[i]);
        furi_string_cat(s, "\n");
        storage_file_write(file, furi_string_get_cstr(s), furi_string_size(s));
        furi_string_free(s);
    }
    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
}

// Encode a Dooya/A-OK 64-bit frame into RAW pulse array
#define NR_RAW_BUF_SIZE 1024
static int16_t nr_raw_buf[NR_RAW_BUF_SIZE];

static uint16_t nr_dooya_encode_raw(int16_t* buf, uint64_t frame, uint8_t repeats) {
    uint16_t pos = 0;
    for(uint8_t r = 0; r < repeats; r++) {
        for(uint8_t i = 0; i < 8; i++) { buf[pos++] = 290; buf[pos++] = -600; }
        buf[pos++] = 5000; buf[pos++] = -650;
        for(int8_t bit = 63; bit >= 0; bit--) {
            if((frame >> bit) & 1) { buf[pos++] = 600; buf[pos++] = -290; }
            else { buf[pos++] = 290; buf[pos++] = -600; }
        }
        buf[pos++] = 290; buf[pos++] = -5000;
    }
    return pos;
}

static void nr_seed(NRApp* a) {
    if(a->dev_count > 0) return; // already have data
    #define SEED(P,TE,ID,HITS,NAME,DATE,FREQ,RSSI) { \
        NRDev* d = &a->devs[a->dev_count++]; memset(d,0,sizeof(NRDev)); \
        d->proto=P; d->te=TE; d->dev_id=ID; d->hits=HITS; d->seeded=true; \
        d->useful=true; d->freq=FREQ; d->rssi=RSSI; snprintf(d->name, NR_MAX_NAME, NAME); \
        snprintf(d->last_seen_date, 12, DATE); }

    SEED(NRProtoHoneywell, 143, 0x5800, 1633, "Alarm System", "Jun 13", 433920000, -85);
    SEED(NRProtoKeeloq, 289, 0x2F9AE15, 24, "Parking Fob", "May 7", 433920000, -88);
    a->devs[a->dev_count-1].sig_count = 2;
    snprintf(a->devs[a->dev_count-1].sigs[0].label, 20, "S2 2F9AE1");
    snprintf(a->devs[a->dev_count-1].sigs[1].label, 20, "S3 2F9AE1");

    SEED(NRProtoPT2262, 194, 0x4F, 53, "Remote 4F", "Jun 9", 433920000, -75);
    NRDev* r = &a->devs[a->dev_count-1];
    memset(r->sigs, 0, sizeof(r->sigs));
    memcpy(r->sigs[0].raw, (uint8_t[]){0xFF,0xFE,0x4F,0xFF,0xE0}, 5);
    r->sigs[0].raw_len = 5; r->sigs[0].bits = 40;
    snprintf(r->sigs[0].label, 20, "Cmd:E0 (Btn A)");
    r->sigs[0].tx_key = 0xFFFE4F;
    memcpy(r->sigs[1].raw, (uint8_t[]){0x00,0x44,0x80}, 3);
    r->sigs[1].raw_len = 3; r->sigs[1].bits = 24;
    snprintf(r->sigs[1].label, 20, "Cmd:22 (Btn B)");
    r->sigs[1].tx_key = 0x004480;
    r->sig_count = 2;

    // Neighbor Gate — Princeton TE=311, 4 buttons
    SEED(NRProtoPT2262, 311, 0x9C, 8, "Neighbor Gate", "Jun 13", 433920000, -87);
    { NRDev* ng = &a->devs[a->dev_count-1];
      memset(ng->sigs, 0, sizeof(ng->sigs));
      snprintf(ng->sigs[0].label, 20, "Open"); ng->sigs[0].tx_key = 0x9CB871; ng->sigs[0].has_file = true; ng->sigs[0].file_seq = 9002;
      snprintf(ng->sigs[1].label, 20, "Close"); ng->sigs[1].tx_key = 0x9CB872; ng->sigs[1].has_file = true; ng->sigs[1].file_seq = 9003;
      snprintf(ng->sigs[2].label, 20, "Pedestrian"); ng->sigs[2].tx_key = 0x9CB874; ng->sigs[2].has_file = true; ng->sigs[2].file_seq = 9004;
      snprintf(ng->sigs[3].label, 20, "Light"); ng->sigs[3].tx_key = 0x9CB878; ng->sigs[3].has_file = true; ng->sigs[3].file_seq = 9005;
      ng->sig_count = 4;
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 71", 311, 9002);
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 72", 311, 9003);
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 74", 311, 9004);
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 78", 311, 9005);
    }

    // Remote C6 — Princeton TE=380, 1 button
    SEED(NRProtoPT2262, 380, 0xC6, 9, "Remote C6", "Jun 13", 433920000, -77);
    { NRDev* rc = &a->devs[a->dev_count-1];
      memset(rc->sigs, 0, sizeof(rc->sigs));
      snprintf(rc->sigs[0].label, 20, "Button"); rc->sigs[0].tx_key = 0xC62C86; rc->sigs[0].has_file = true; rc->sigs[0].file_seq = 9040;
      rc->sig_count = 1;
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 C6 2C 86", 380, 9040);
    }

    SEED(NRProtoFSK, 65, 0xF5C0, 118, "FSK Sensor", "Apr 30", 433920000, -88);
    SEED(NRProtoEV1527, 113, 0x87FFE, 1, "Sens 87FFE", "May 8", 433920000, -90);
    SEED(NRProtoPT2262, 322, 0xEA, 1, "Remote EA", "Jun 9", 433920000, -86);
    { NRDev* ea = &a->devs[a->dev_count-1];
      memset(ea->sigs, 0, sizeof(ea->sigs));
      snprintf(ea->sigs[0].label, 20, "Button"); ea->sigs[0].tx_key = 0xEA55B1; ea->sigs[0].has_file = true; ea->sigs[0].file_seq = 9041;
      ea->sig_count = 1;
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 EA 55 B1", 322, 9041);
    }
    SEED(NRProtoPT2262, 311, 0x11, 2, "Remote 11", "Jun 9", 433920000, -81);
    { NRDev* r11 = &a->devs[a->dev_count-1];
      memset(r11->sigs, 0, sizeof(r11->sigs));
      snprintf(r11->sigs[0].label, 20, "Button"); r11->sigs[0].tx_key = 0x11B172; r11->sigs[0].has_file = true; r11->sigs[0].file_seq = 9042;
      r11->sig_count = 1;
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 11 B1 72", 311, 9042);
    }

    // Gate remote — Princeton tristate X1XX0XX000Z1 (captured via RTL-SDR)
    SEED(NRProtoPT2262, 340, 0x75, 1, "Gate", "Jun 12", 433920000, -80);
    { NRDev* gt = &a->devs[a->dev_count-1];
      memset(gt->sigs, 0, sizeof(gt->sigs));
      snprintf(gt->sigs[0].label, 20, "Open/Close"); gt->sigs[0].tx_key = 0x75140B;
      gt->sig_count = 1;
    }

    SEED(NRProtoPT2262, 340, 0xF7, 1, "Gate 2", "Jun 12", 433920000, -97);
    { NRDev* g2 = &a->devs[a->dev_count-1];
      memset(g2->sigs, 0, sizeof(g2->sigs));
      snprintf(g2->sigs[0].label, 20, "Open/Close"); g2->sigs[0].tx_key = 0xF72C9E;
      g2->sig_count = 1;
    }

    // Confirmed neighbor KeeLoq fobs (seen multiple times across batches)
    SEED(NRProtoKeeloq, 225, 0x000B116, 8, "Fob B116", "May 2", 433920000, -92);
    SEED(NRProtoKeeloq, 295, 0x008011F, 6, "Fob 8011F", "May 8", 433920000, -92);
    SEED(NRProtoKeeloq, 295, 0x008005E, 5, "Fob 8005E", "May 25", 433920000, -92);
    SEED(NRProtoKeeloq, 240, 0x000B118, 3, "Fob B118", "May 2", 433920000, -92);
    SEED(NRProtoKeeloq, 300, 0x0080218, 4, "Fob 80218", "May 25", 433920000, -85);
    SEED(NRProtoBinRAW, 98, 0xB109, 699, "OOK Unknown 98", "Jun 13", 433920000, -84);
    SEED(NRProtoNexusTH, 650, 0xE0E0, 29, "Weather E0", "Jun 12", 433920000, -88);
    a->devs[a->dev_count-1].sig_count = 1;
    snprintf(a->devs[a->dev_count-1].sigs[0].label, 20, "16.5C");
    SEED(NRProtoBinRAW, 345, 0xB122, 10, "Bell Ctrl", "May 2", 433920000, -83);

    // Interlogix/GE alarm system — 7 sensors (motion+smoke+contact), event-only TX
    SEED(NRProtoBinRAW, 366, 0xB14A, 7, "Interlogix", "Jun 12", 433920000, -90);

    // TPMS — known car tire sensors (captured via RTL-SDR)
    SEED(NRProtoTPMS, 120, 0x09B6BE9, 1, "V Schrader", "Jun 4", 433920000, -80);
    SEED(NRProtoTPMS, 48, 0xD76C7040, 16, "N1 Toyota", "Jun 7", 433920000, -112);
    SEED(NRProtoTPMS, 48, 0xD76CA970, 1, "N1 Toyota b", "Jun 4", 433920000, -120);
    SEED(NRProtoTPMS, 48, 0xF13D76AC, 5, "N2 Toyota", "Jun 9", 433920000, -51);
    SEED(NRProtoTPMS, 48, 0x8147E6, 5, "N3 Renault", "Jun 12", 433920000, -86);
    SEED(NRProtoTPMS, 48, 0xD769369B, 7, "R1 Toyota", "Jun 12", 433920000, -120);
    SEED(NRProtoTPMS, 48, 0x4636F918, 6, "R2 Ford", "Jun 6", 433920000, -78);
    SEED(NRProtoTPMS, 48, 0x07CCA0, 3, "R3 Renault", "Jun 12", 433920000, -23);
    SEED(NRProtoTPMS, 48, 0x85C4975C, 2, "V Citroen", "Jun 4", 433920000, -120);
    SEED(NRProtoTPMS, 120, 0x4B87E0, 5, "V Schrader2", "Jun 9", 433920000, -55);

    // Markisol blind remote — OOK_PWM s=368 l=704 sync=5628, 40-bit fixed code
    // ID=0x0100, captured Jun 7 16:18. Replayable (no rolling code).
    SEED(NRProtoBinRAW, 368, 0x0100, 1, "Markisol", "Jun 12", 433920000, -97);
    { NRDev* mk = &a->devs[a->dev_count-1];
      memset(mk->sigs, 0, sizeof(mk->sigs));
      snprintf(mk->sigs[0].label, 20, "Up"); mk->sigs[0].tx_key = 0x0100C0013FULL;
      snprintf(mk->sigs[1].label, 20, "Down"); mk->sigs[1].tx_key = 0x01001001EFULL;
      snprintf(mk->sigs[2].label, 20, "Stop"); mk->sigs[2].tx_key = 0x01005001AFULL;
      mk->sig_count = 3;
    }

    // UniFan-24V ceiling fan — OOK_PWM s=256 l=756 sync=3616, 33-bit with 3-bit counter
    // ID=0x47864 (292964), captured Jun 9 00:10. Counter=0 for all.
    SEED(NRProtoBinRAW, 256, 0x47864, 4, "Fan", "Jun 12", 433920000, -121);
    { NRDev* fan = &a->devs[a->dev_count-1];
      memset(fan->sigs, 0, sizeof(fan->sigs));
      snprintf(fan->sigs[0].label, 20, "Speed 1"); fan->sigs[0].tx_key = 0x08F0C8F19ULL;
      snprintf(fan->sigs[1].label, 20, "Speed 4"); fan->sigs[1].tx_key = 0x08F0C9503ULL;
      snprintf(fan->sigs[2].label, 20, "Fan Off"); fan->sigs[2].tx_key = 0x08F0C891FULL;
      snprintf(fan->sigs[3].label, 20, "Light"); fan->sigs[3].tx_key = 0x08F0C9701ULL;
      fan->sig_count = 4;
    }

    // FSK key fobs (captured via RTL-SDR, rolling code, monitor-only)
    SEED(NRProtoFSK, 48, 0xEFF864, 1, "Honda", "Jun 6", 433920000, -70);

    // 868 MHz devices
    SEED(NRProtoBinRAW, 320, 0x09EC, 19, "Garage", "Jun 9", 868350000, -75);
    { NRDev* g = &a->devs[a->dev_count-1];
      memset(g->sigs, 0, sizeof(g->sigs));
      memcpy(g->sigs[0].raw, (uint8_t[]){0x9E,0xC0}, 2);
      g->sigs[0].raw_len = 2; g->sigs[0].bits = 12;
      snprintf(g->sigs[0].label, 20, "CAME 0x9EC");
      g->sigs[0].file_seq = 9006; g->sigs[0].has_file = true;
      g->sigs[0].tx_key = 0x09EC;  // 12-bit CAME key
      g->sig_count = 1;
      nr_seed_sub(a, "CAME", 868350000, 12, "00 00 00 00 00 00 09 EC", 0, 9006);
    }
    SEED(NRProtoBinRAW, 249, 0xB118, 2, "868 Dev 249", "May 16", 868350000, -85);

    // Dooya Windows — 3 remotes with direct TX encoding
    SEED(NRProtoBinRAW, 366, 0xC0A16C, 0, "Window 1", "Jun 12", 433920000, 0);
    { NRDev* w = &a->devs[a->dev_count-1];
      snprintf(w->sigs[0].label, 20, "UP"); w->sigs[0].tx_key = 0xA3C0A16C01000BD9ULL; w->sigs[0].has_file = true; w->sigs[0].file_seq = 9010;
      snprintf(w->sigs[1].label, 20, "STOP"); w->sigs[1].tx_key = 0xA3C0A16C010023F1ULL; w->sigs[1].has_file = true; w->sigs[1].file_seq = 9011;
      snprintf(w->sigs[2].label, 20, "DOWN"); w->sigs[2].tx_key = 0xA3C0A16C01004311ULL; w->sigs[2].has_file = true; w->sigs[2].file_seq = 9012;
      w->sig_count = 3;
      uint16_t n;
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C0A16C01000BD9ULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9010);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C0A16C010023F1ULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9011);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C0A16C01004311ULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9012);
    }
    SEED(NRProtoBinRAW, 366, 0xC0AD01, 0, "Window 2", "Jun 12", 433920000, 0);
    { NRDev* w = &a->devs[a->dev_count-1];
      snprintf(w->sigs[0].label, 20, "UP"); w->sigs[0].tx_key = 0xA3C0AD0101000B7AULL; w->sigs[0].has_file = true; w->sigs[0].file_seq = 9020;
      snprintf(w->sigs[1].label, 20, "STOP"); w->sigs[1].tx_key = 0xA3C0AD01010023B2ULL; w->sigs[1].has_file = true; w->sigs[1].file_seq = 9021;
      snprintf(w->sigs[2].label, 20, "DOWN"); w->sigs[2].tx_key = 0xA3C0AD01010043B2ULL; w->sigs[2].has_file = true; w->sigs[2].file_seq = 9022;
      w->sig_count = 3;
      uint16_t n;
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C0AD0101000B7AULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9020);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C0AD01010023B2ULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9021);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C0AD01010043B2ULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9022);
    }
    SEED(NRProtoBinRAW, 366, 0xC09EBD, 0, "Window 3", "Jun 12", 433920000, 0);
    { NRDev* w = &a->devs[a->dev_count-1];
      snprintf(w->sigs[0].label, 20, "UP"); w->sigs[0].tx_key = 0xA3C09EBD01000B27ULL; w->sigs[0].has_file = true; w->sigs[0].file_seq = 9030;
      snprintf(w->sigs[1].label, 20, "STOP"); w->sigs[1].tx_key = 0xA3C09EBD0100233FULL; w->sigs[1].has_file = true; w->sigs[1].file_seq = 9031;
      snprintf(w->sigs[2].label, 20, "DOWN"); w->sigs[2].tx_key = 0xA3C09EBD0100435FULL; w->sigs[2].has_file = true; w->sigs[2].file_seq = 9032;
      w->sig_count = 3;
      uint16_t n;
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C09EBD01000B27ULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9030);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C09EBD0100233FULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9031);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3C09EBD0100435FULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9032);
    }
    SEED(NRProtoBinRAW, 366, 0x635A4B, 2, "Window 4", "Jun 12", 433920000, -121);
    { NRDev* w = &a->devs[a->dev_count-1];
      snprintf(w->sigs[0].label, 20, "UP"); w->sigs[0].tx_key = 0xA3635A4B01000B14ULL; w->sigs[0].has_file = true; w->sigs[0].file_seq = 9050;
      snprintf(w->sigs[1].label, 20, "STOP"); w->sigs[1].tx_key = 0xA3635A4B0100232CULL; w->sigs[1].has_file = true; w->sigs[1].file_seq = 9051;
      snprintf(w->sigs[2].label, 20, "DOWN"); w->sigs[2].tx_key = 0xA3635A4B0100434CULL; w->sigs[2].has_file = true; w->sigs[2].file_seq = 9052;
      w->sig_count = 3;
      uint16_t n;
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3635A4B01000B14ULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9050);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3635A4B0100232CULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9051);
      n = nr_dooya_encode_raw(nr_raw_buf, 0xA3635A4B0100434CULL, 3); nr_seed_raw_sub(a, 433920000, nr_raw_buf, n, 9052);
    }
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

// Dooya A-OK 64-bit frame handler
static void nr_dooya_rx_frame(NRApp* a, uint64_t frame) {
    uint32_t rid = (frame >> 32) & 0xFFFFFF;
    int8_t di = -1;
    for(uint8_t i = 0; i < a->dev_count; i++) {
        if(a->devs[i].dev_id == rid && a->devs[i].freq == 433920000 &&
           (a->devs[i].proto == NRProtoBinRAW || a->devs[i].proto == NRProtoPT2262) &&
           a->devs[i].sigs[0].has_file) { di = i; break; }
    }
    if(di < 0) return;
    NRDev* d = &a->devs[di];
    uint32_t hash = (uint32_t)(frame >> 16);
    if(hash == a->dooya_last_hash && (a->tick - d->last_seen) < 4) return;
    a->dooya_last_hash = hash;
    if((a->tick - d->last_seen) >= NR_HIT_COOLDOWN) d->hits++;
    d->last_seen = a->tick;
    d->confirmed = true;
    d->rssi = furi_hal_subghz_get_rssi();
    nr_update_date(d);
    // Autosave Dooya frame
    uint8_t raw[8];
    for(int i = 7; i >= 0; i--) { raw[i] = frame & 0xFF; frame >>= 8; }
    NRSig tmp = {.raw_len = 8, .bits = 64};
    memcpy(tmp.raw, raw, 8);
    snprintf(tmp.label, 20, "Dooya %06lX", (unsigned long)rid);
    nr_autosave_sig(a, d, &tmp);
}

// NexusTH state machine decoder (gap-based PPM)
// Protocol: ~490µs pulse + gap (980µs=0, 1960µs=1, 3920µs=sync)
static void nr_nexus_rx_frame(NRApp* a, uint64_t data) {
    // Validate: nibble 6 must be 0xF
    if(((data >> 8) & 0x0F) != 0x0F) return;
    uint8_t id = (data >> 28) & 0xFF;
    int16_t raw_temp = (data >> 12) & 0x0FFF;
    if(raw_temp & 0x0800) raw_temp |= 0xF000; // sign extend
    if(raw_temp < -400 || raw_temp > 600) return; // -40C to +60C
    uint8_t hum = data & 0xFF;
    if(hum > 100) return;
    // Build fake frame for nr_process: pack as NexusTH-style data
    uint8_t buf[5];
    buf[0] = id;
    buf[1] = (data >> 20) & 0xFF; // flags + temp high
    buf[2] = (data >> 12) & 0xFF; // temp low
    buf[3] = hum;
    buf[4] = 0;
    // Format label
    float temp_f = raw_temp / 10.0f;
    char label[20];
    snprintf(label, 20, "%.1fC %d%%", (double)temp_f, hum);
    // Find or create device
    uint32_t dev_id = 0xE000 | (uint32_t)id; // matches nr_dev_id for NexusTH
    NRDev* d = NULL;
    for(uint8_t i = 0; i < a->dev_count; i++) {
        if(a->devs[i].proto == NRProtoNexusTH && a->devs[i].dev_id == dev_id) {
            d = &a->devs[i]; break;
        }
    }
    if(!d && a->dev_count < NR_MAX_DEVICES) {
        d = &a->devs[a->dev_count++];
        memset(d, 0, sizeof(NRDev));
        d->proto = NRProtoNexusTH;
        d->dev_id = dev_id;
        d->te = 500;
        d->freq = a->rx_freq;
        snprintf(d->name, NR_MAX_NAME, "Weather %02X", id);
        d->useful = true;
    }
    if(!d) return;
    d->hits++;
    d->last_seen = a->tick;
    d->rssi = furi_hal_subghz_get_rssi();
    d->confirmed = true;
    snprintf(d->sigs[0].label, 20, "%s", label);
    d->sig_count = 1;
    memcpy(d->sigs[0].raw, buf, 4);
    d->sigs[0].raw_len = 4;
    d->sigs[0].bits = 36;
    nr_update_date(d);
    // Autosave NexusTH frame
    nr_autosave_sig(a, d, &d->sigs[0]);
}

// NexusTH / GT-WT02 state machine decoder (gap-based PPM)
// GT-WT02: pulse ~500µs, gap 2000µs=0, gap 4000µs=1, sync gap 9000µs
// Nexus-TH: pulse ~500µs, gap 1000µs=0, gap 2000µs=1, sync gap 4000µs
// We accept BOTH timing variants with wide tolerance windows
static void nr_nexus_decode(NRApp* a, bool level, uint32_t duration) {
    switch(a->nexus_state) {
    case 0: // wait for sync gap (4000-10000µs covers both protocols)
        if(!level && duration > 3500 && duration < 10500) {
            a->nexus_bits = 0;
            a->nexus_data = 0;
            a->nexus_state = 1;
        }
        break;
    case 1: // expect pulse (~500µs)
        if(level && duration > 250 && duration < 750) {
            a->nexus_pulse = duration;
            a->nexus_state = 2;
        } else {
            a->nexus_state = 0;
        }
        break;
    case 2: // check gap duration
        if(!level) {
            if(duration > 3500 && duration < 10500) {
                // Sync gap — frame complete
                if(a->nexus_bits == 36 || a->nexus_bits == 37) {
                    nr_nexus_rx_frame(a, a->nexus_data);
                }
                a->nexus_bits = 0;
                a->nexus_data = 0;
                a->nexus_state = 1;
            } else if(duration > 700 && duration < 2700) {
                // Short gap: Nexus 980µs or GT-WT02 2000µs = bit 0
                a->nexus_data = (a->nexus_data << 1);
                a->nexus_bits++;
                a->nexus_state = 1;
            } else if(duration > 2700 && duration < 5500) {
                // Long gap: Nexus 1960µs or GT-WT02 4000µs = bit 1
                a->nexus_data = (a->nexus_data << 1) | 1;
                a->nexus_bits++;
                a->nexus_state = 1;
            } else {
                a->nexus_state = 0;
            }
        } else {
            a->nexus_state = 0;
        }
        break;
    }
}

// Dooya A-OK 64-bit RX state machine
static void nr_dooya_decode(NRApp* a, bool level, uint32_t duration) {
    switch(a->dooya_state) {
    case 0: // idle
        if(level && duration > 180 && duration < 450) {
            a->dooya_pre = 1; a->dooya_state = 1;
        }
        break;
    case 1: // preamble
        if(!level && duration > 400 && duration < 800) { /* gap ok */ }
        else if(level && duration > 180 && duration < 450) { a->dooya_pre++; }
        else if(level && duration > 3500 && duration < 6500) {
            a->dooya_state = a->dooya_pre >= 4 ? 2 : 0;
        } else { a->dooya_state = 0; }
        break;
    case 2: // sync gap
        if(!level && duration > 300 && duration < 1000) {
            a->dooya_bits = 0; a->dooya_data = 0; a->dooya_state = 3;
        } else { a->dooya_state = 0; }
        break;
    case 3: // data
        if(level) {
            a->dooya_data <<= 1;
            if(duration > 400) a->dooya_data |= 1;
            if(++a->dooya_bits >= 64) {
                nr_dooya_rx_frame(a, a->dooya_data);
                a->dooya_state = 0;
            }
        } else if(duration > 3500) {
            a->dooya_state = 0;
        }
        break;
    }
}

// Schrader TPMS Manchester decoder (TE=120/240µs, 64-bit, CRC-8)
static void nr_tpms_rx_frame(NRApp* a, uint64_t data) {
    // CRC-8 check (poly 0x07)
    uint8_t msg[6] = {data>>48, data>>40, data>>32, data>>24, data>>16, data>>8};
    uint8_t crc = 0;
    for(int i = 0; i < 6; i++) {
        crc ^= msg[i];
        for(int b = 0; b < 8; b++) crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : crc << 1;
    }
    if(crc != (uint8_t)(data & 0xFF)) return;

    uint32_t id = (uint32_t)(data >> 24);
    int8_t temp = (int8_t)(((data >> 8) & 0xFF) - 50);
    uint8_t pres_raw = (data >> 16) & 0xFF;
    float bar = pres_raw * 2.5f * 0.069f;

    // Find or create device
    NRDev* d = NULL;
    for(uint8_t i = 0; i < a->dev_count; i++) {
        if(a->devs[i].proto == NRProtoTPMS && a->devs[i].dev_id == id) { d = &a->devs[i]; break; }
    }
    if(!d && a->dev_count < NR_MAX_DEVICES) {
        d = &a->devs[a->dev_count++];
        memset(d, 0, sizeof(NRDev));
        d->proto = NRProtoTPMS;
        d->dev_id = id;
        d->te = 120;
        d->freq = a->rx_freq;
        snprintf(d->name, NR_MAX_NAME, "Car %06lX", (unsigned long)(id & 0xFFFFFF));
        d->useful = true;
    }
    if(!d) return;
    d->hits++;
    d->last_seen = a->tick;
    d->rssi = furi_hal_subghz_get_rssi();
    d->confirmed = true;
    snprintf(d->sigs[0].label, 20, "%dC %.1fbar", temp, (double)bar);
    d->sig_count = 1;
    nr_update_date(d);
}

static void nr_tpms_decode(NRApp* a, bool level, uint32_t duration) {
    // Manchester decoder for Schrader GG4: TE_short=120, TE_long=240, delta=55
    switch(a->tpms_state) {
    case 0: // wait for start pulse (~480µs HIGH = 2*TE_long)
        if(level && duration > 380 && duration < 580) {
            a->tpms_state = 1;
            a->tpms_pre = 0;
            a->tpms_bit_cnt = 0;
            a->tpms_data = 0;
            a->tpms_manch = 0; // ManchesterStateStart1
            a->tpms_last_level = level;
            a->tpms_last_dur = duration;
        }
        break;
    case 1: // preamble + data via Manchester
    case 2:
        {
        // Classify duration
        uint8_t sym; // 0=short, 1=long, 2=reset
        if(duration > 65 && duration < 175) sym = 0; // short (~120µs)
        else if(duration > 185 && duration < 295) sym = 1; // long (~240µs)
        else { a->tpms_state = 0; break; } // out of tolerance

        // Simple Manchester II decode using edge counting
        // Long = full bit period = emit bit; Short = half period = wait for pair
        bool emit = false;
        bool bit_val = false;
        if(sym == 1) { // long duration = full symbol
            emit = true;
            bit_val = !level; // Manchester II: falling edge in middle = 1
        } else { // short duration
            if(a->tpms_manch == 0) { a->tpms_manch = 1; break; } // first half, wait
            a->tpms_manch = 0; // second half
            emit = true;
            bit_val = !level;
        }

        if(!emit) break;

        if(a->tpms_state == 1) {
            // Preamble: expect zeros
            if(bit_val != false) { a->tpms_state = 0; break; }
            if(++a->tpms_pre >= 3) a->tpms_state = 2;
        } else {
            // Data collection
            a->tpms_data = (a->tpms_data << 1) | (bit_val ? 1 : 0);
            if(++a->tpms_bit_cnt >= 64) {
                nr_tpms_rx_frame(a, a->tpms_data);
                a->tpms_state = 0;
            }
        }
        }
        break;
    }
}

static void nr_rx_cb(void* ctx, bool level, uint32_t duration) {
    NRApp* a = ctx;
    // Feed firmware protocol decoders
    subghz_receiver_decode(a->receiver, level, duration);
    // Feed Dooya A-OK 64-bit decoder
    nr_dooya_decode(a, level, duration);
    // Feed NexusTH gap-based decoder
    nr_nexus_decode(a, level, duration);
    // Feed Schrader TPMS Manchester decoder
    nr_tpms_decode(a, level, duration);
    if(level) { a->rx_pulse = duration; return; }
    uint32_t h = a->rx_pulse, l = duration;
    // Frame-end gap: 5ms for OOK, 1.5ms for FSK
    bool is_fm = (a->freq_mode == NRFreq433FM || a->freq_mode == NRFreq868FM);
    uint32_t gap_threshold = is_fm ? 800 : 5000;
    uint16_t min_bits = is_fm ? 40 : 24;
    if(l > gap_threshold) {
        if(a->rx_bit_count >= min_bits && a->rx_te_n > 0 && !a->rx_ready) {
            uint16_t te = a->rx_te_sum / a->rx_te_n;
            uint8_t bl = (a->rx_bit_count + 7) / 8; if(bl > 32) bl = 32;
            uint8_t tmp[32];
            memset(tmp, 0, 32);
            for(uint16_t i = 0; i < a->rx_bit_count && i < 256; i++)
                if(a->rx_bits[i]) tmp[i/8] |= (1 << (7-(i%8)));
            // FM noise filter: reject frames with >75% ones (idle mark frequency)
            // Also reject if TE is outside FSK range (40-200µs = 5-25kbps)
            if(is_fm) {
                if(te < 40 || te > 200) {
                    a->rx_bit_count = 0; a->rx_te_sum = 0; a->rx_te_n = 0;
                    return;
                }
                uint16_t ones = 0;
                for(uint16_t i = 0; i < a->rx_bit_count; i++) if(a->rx_bits[i]) ones++;
                if(ones > a->rx_bit_count * 3 / 4 || ones < a->rx_bit_count / 4) {
                    a->rx_bit_count = 0; a->rx_te_sum = 0; a->rx_te_n = 0;
                    return;
                }
            }
            // Repeat validation: EV1527/PT2262 range requires 2 identical frames
            bool need_repeat = (te >= 105 && te <= 400 && a->rx_bit_count <= 56);
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
    bool is_fm = (a->freq_mode == NRFreq433FM || a->freq_mode == NRFreq868FM);
    if(is_fm)
        subghz_devices_load_preset(a->radio, FuriHalSubGhzPreset2FSKDev238Async, NULL);
    else
        subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
    uint32_t freq = nr_freq_vals[a->freq_mode];
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

// ============== Direct TX Encoders (same method as Dooya Remote app) ==============

static LevelDuration nr_tx_yield(void* ctx) {
    NRApp* a = ctx;
    if(a->upload_idx >= a->upload_size) return level_duration_reset();
    return a->upload[a->upload_idx++];
}

// Princeton/PT2262/EV1527: sync + 24-bit PWM
static uint16_t nr_encode_princeton(LevelDuration* buf, uint16_t pos, uint32_t key, uint16_t te) {
    uint16_t te3 = te * 3;
    // Sync: HIGH te, LOW 31*te
    buf[pos++] = level_duration_make(true, te);
    buf[pos++] = level_duration_make(false, (uint32_t)te * 31);
    // Data: 24 bits MSB first
    for(int8_t i = 23; i >= 0; i--) {
        if((key >> i) & 1) {
            buf[pos++] = level_duration_make(true, te3);
            buf[pos++] = level_duration_make(false, te);
        } else {
            buf[pos++] = level_duration_make(true, te);
            buf[pos++] = level_duration_make(false, te3);
        }
    }
    return pos;
}

// CAME: header + 12-bit PWM
static uint16_t nr_encode_came(LevelDuration* buf, uint16_t pos, uint16_t key, uint16_t te) {
    // Header: LOW 47*te, HIGH te
    buf[pos++] = level_duration_make(false, (uint32_t)te * 47);
    buf[pos++] = level_duration_make(true, te);
    // Data: 12 bits MSB first
    for(int8_t i = 11; i >= 0; i--) {
        if((key >> i) & 1) {
            buf[pos++] = level_duration_make(false, (uint32_t)te * 2);
            buf[pos++] = level_duration_make(true, te);
        } else {
            buf[pos++] = level_duration_make(false, te);
            buf[pos++] = level_duration_make(true, (uint32_t)te * 2);
        }
    }
    return pos;
}

// Dooya/A-OK: preamble + sync + 64-bit PWM + gap
static uint16_t nr_encode_dooya(LevelDuration* buf, uint16_t pos, uint64_t frame) {
    // Preamble: 8x (HIGH 290, LOW 600)
    for(uint8_t i = 0; i < 8; i++) {
        buf[pos++] = level_duration_make(true, 290);
        buf[pos++] = level_duration_make(false, 600);
    }
    // Sync: HIGH 5000, LOW 650
    buf[pos++] = level_duration_make(true, 5000);
    buf[pos++] = level_duration_make(false, 650);
    // Data: 64 bits MSB first
    for(int8_t bit = 63; bit >= 0; bit--) {
        if((frame >> bit) & 1) {
            buf[pos++] = level_duration_make(true, 600);
            buf[pos++] = level_duration_make(false, 290);
        } else {
            buf[pos++] = level_duration_make(true, 290);
            buf[pos++] = level_duration_make(false, 600);
        }
    }
    // Gap
    buf[pos++] = level_duration_make(true, 290);
    buf[pos++] = level_duration_make(false, 5000);
    return pos;
}

// Markisol: sync(4886h+2470l+1647h+315l) + 40-bit PWM (0=670h/320l, 1=348h/642l)
// BUT: raw air bits are inverted+reversed vs decoded frame. We must transform.
static uint16_t nr_encode_markisol(LevelDuration* buf, uint16_t pos, uint64_t frame) {
    // Transform decoded frame bytes to air bits (reverse of rtl_433 decode)
    uint8_t dec[5];
    for(int i = 0; i < 5; i++) dec[i] = (frame >> (32 - i*8)) & 0xFF;
    // Reverse decode: air_byte = reverse8(~dec_byte), then shift left by 1 across all
    uint8_t air[6] = {0};
    for(int i = 0; i < 5; i++) {
        uint8_t r = 0;
        uint8_t v = ~dec[i];
        for(int b = 0; b < 8; b++) r |= ((v >> b) & 1) << (7-b);
        air[i] = r;
    }
    // Shift right by 1 bit (undo the <<1 in decode)
    for(int i = 4; i >= 0; i--) {
        air[i+1] |= (air[i] & 1) << 7;
        air[i] >>= 1;
    }
    air[0] |= 0x80; // first spurious bit is always 1

    // Sync: 4886h, 2470l, 1647h, 315l
    buf[pos++] = level_duration_make(true, 4886);
    buf[pos++] = level_duration_make(false, 2470);
    buf[pos++] = level_duration_make(true, 1647);
    buf[pos++] = level_duration_make(false, 315);
    // 41 bits (40 data + 1 spurious first bit already in air[])
    for(int i = 0; i < 41; i++) {
        uint8_t bit = (air[i/8] >> (7 - (i%8))) & 1;
        if(bit) { // one: short high, long low
            buf[pos++] = level_duration_make(true, 348);
            buf[pos++] = level_duration_make(false, 642);
        } else { // zero: long high, short low
            buf[pos++] = level_duration_make(true, 670);
            buf[pos++] = level_duration_make(false, 320);
        }
    }
    // Inter-frame gap (must exceed reset_limit=2000 for rtl_433 to separate rows)
    buf[pos++] = level_duration_make(true, 348);
    buf[pos++] = level_duration_make(false, 8000);
    return pos;
}

// UniFan-24V: sync(3616h) + 33-bit PWM (0=756h/252l, 1=256h/756l) + gap 8200
static uint16_t nr_encode_unifan(LevelDuration* buf, uint16_t pos, uint64_t frame33) {
    buf[pos++] = level_duration_make(true, 3616);
    buf[pos++] = level_duration_make(false, 252);
    for(int8_t i = 32; i >= 0; i--) {
        if((frame33 >> i) & 1) {
            buf[pos++] = level_duration_make(true, 256);
            buf[pos++] = level_duration_make(false, 756);
        } else {
            buf[pos++] = level_duration_make(true, 756);
            buf[pos++] = level_duration_make(false, 252);
        }
    }
    buf[pos++] = level_duration_make(true, 256);
    buf[pos++] = level_duration_make(false, 8200);
    return pos;
}

static void nr_tx(NRApp* a, NRDev* d, NRSig* s) {
    if(!s->tx_key && !s->raw_len) return;
    bool was = a->rx_on; if(was) nr_rx_stop(a);

    uint16_t pos = 0;
    uint32_t freq = d->freq ? d->freq : 433920000;

    if(s->tx_key) {
        // Direct in-memory encoding — fast path (like Dooya Remote app)
        if(d->proto == NRProtoBinRAW && (d->dev_id == 0xC0A16C || d->dev_id == 0xC0AD01 || d->dev_id == 0xC09EBD || d->dev_id == 0x635A4B)) {
            // Dooya: 3 repeats + CONFIRM after UP/DOWN (not STOP)
            for(uint8_t r = 0; r < 3; r++)
                pos = nr_encode_dooya(a->upload, pos, s->tx_key);
            uint8_t cmd = (s->tx_key >> 8) & 0xFF;
            if(cmd == 0x0B || cmd == 0x43) { // UP or DOWN
                uint64_t confirm = (s->tx_key & 0xFFFFFFFFFFFF0000ULL) | 0x2400 |
                    (((s->tx_key >> 48) + ((s->tx_key >> 40) & 0xFF) + ((s->tx_key >> 32) & 0xFF) +
                      ((s->tx_key >> 24) & 0xFF) + ((s->tx_key >> 16) & 0xFF) + 0x24) & 0xFF);
                for(uint8_t r = 0; r < 3; r++)
                    pos = nr_encode_dooya(a->upload, pos, confirm);
            }
        } else if(freq == 868350000 && s->bits == 12) {
            // CAME 12-bit: 6 repeats
            for(uint8_t r = 0; r < 6; r++)
                pos = nr_encode_came(a->upload, pos, (uint16_t)s->tx_key, 320);
        } else if(d->dev_id == 0x0100 && d->te >= 350 && d->te <= 400) {
            // Markisol: 7 repeats (per protocol spec)
            for(uint8_t r = 0; r < 7; r++)
                pos = nr_encode_markisol(a->upload, pos, s->tx_key);
        } else if(d->dev_id == 0x47864) {
            // UniFan: 7 repeats
            for(uint8_t r = 0; r < 7; r++)
                pos = nr_encode_unifan(a->upload, pos, s->tx_key);
        } else {
            // Princeton/PT2262/EV1527: 6 repeats
            for(uint8_t r = 0; r < 6; r++)
                pos = nr_encode_princeton(a->upload, pos, (uint32_t)s->tx_key, d->te);
        }
    } else if(s->raw_len) {
        // Legacy fallback: encode from raw bytes
        uint16_t te = d->te;
        bool is_came = (freq == 868350000 && s->bits == 12 && te >= 280 && te <= 360);
        uint8_t repeats = 6;
        for(uint8_t r = 0; r < repeats && pos < 850; r++) {
            if(is_came) {
                pos = nr_encode_came(a->upload, pos, (uint16_t)((s->raw[0] << 8) | s->raw[1]) >> 4, te);
            } else {
                // Reconstruct key from raw bytes (24-bit)
                uint32_t key = 0;
                for(uint16_t i = 0; i < s->bits && i < 24; i++)
                    key |= (uint32_t)((s->raw[i/8] >> (7-(i%8))) & 1) << (23-i);
                pos = nr_encode_princeton(a->upload, pos, key, te);
            }
        }
    }

    if(pos == 0) { if(was) nr_rx_start(a); return; }

    // Transmit directly from memory — identical to Dooya Remote app
    a->upload_size = pos;
    a->upload_idx = 0;
    subghz_devices_idle(a->radio);
    subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(a->radio, freq);
    if(subghz_devices_start_async_tx(a->radio, nr_tx_yield, a)) {
        while(!subghz_devices_is_async_complete_tx(a->radio)) furi_delay_ms(10);
        subghz_devices_stop_async_tx(a->radio);
    }
    subghz_devices_idle(a->radio);
    if(was) nr_rx_start(a);
}

// ============== CAME Scan ==============
static void nr_came_tx_code(NRApp* a, uint16_t code) {
    a->came_tx = true;
    bool was = a->rx_on; if(was) nr_rx_stop(a);

    uint16_t pos = 0;
    for(uint8_t r = 0; r < 3; r++)
        pos = nr_encode_came(a->upload, pos, code, 320);

    a->upload_size = pos;
    a->upload_idx = 0;
    subghz_devices_idle(a->radio);
    subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(a->radio, 868350000);
    if(subghz_devices_start_async_tx(a->radio, nr_tx_yield, a)) {
        while(!subghz_devices_is_async_complete_tx(a->radio)) furi_delay_ms(10);
        subghz_devices_stop_async_tx(a->radio);
    }
    subghz_devices_idle(a->radio);
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
                        nr_update_date(d);
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
        // Princeton firmware decode: extract key and match to PT2262 seed
        if(strncmp(a->dec_proto, "Princeton", 9) == 0) {
            char* kp = strstr(a->dec_str, "Key:0x");
            if(kp) {
                uint32_t key = strtoul(kp + 6, NULL, 16);
                uint8_t did = (key >> 16) & 0xFF; // PT2262 dev_id = first byte (matches d[0])
                int8_t di = nr_find_dev(a, NRProtoPT2262, did);
                if(di >= 0) {
                    NRDev* d = &a->devs[di];
                    if((a->tick - d->last_seen) >= NR_HIT_COOLDOWN) {
                        d->hits++; d->last_seen = a->tick;
                        d->rssi = furi_hal_subghz_get_rssi();
                        nr_update_date(d);
                    }
                    if(d->seeded) d->confirmed = true;
                    NRSig tmp = {.raw_len = 3, .bits = 24};
                    tmp.raw[0] = (key >> 16) & 0xFF;
                    tmp.raw[1] = (key >> 8) & 0xFF;
                    tmp.raw[2] = key & 0xFF;
                    snprintf(tmp.label, 20, "Cmd:%02lX", (unsigned long)(key & 0xFF));
                    nr_autosave_sig(a, d, &tmp);
                }
                a->dec_ready = false; a->dec_proto[0] = 0; a->dec_str[0] = 0;
                return;
            }
        }
        // For other firmware decodes, just note it for autosave annotation (handled below)
    }

    if(!a->rx_ready) return;
    uint16_t te = a->rx_fte, bits = a->rx_fbits;
    uint8_t len = a->rx_flen, data[32];
    memcpy(data, a->rx_fdata, len);
    a->rx_ready = false;

    // Toyota TPMS decoder (FSK differential Manchester, ~48µs TE, 64-bit payload)
    bool is_fm = (a->freq_mode == NRFreq433FM || a->freq_mode == NRFreq868FM);
    if(is_fm && te >= 35 && te <= 65 && bits >= 80) {
        // Search for sync pattern 001111 in raw bits
        for(uint16_t si = 0; si + 6 + 128 <= bits; si++) {
            if(a->rx_fdata[si/8] & (0x80 >> (si%8))) continue; // bit[si] must be 0
            if(a->rx_fdata[(si+1)/8] & (0x80 >> ((si+1)%8))) continue; // bit[si+1] must be 0
            bool sync_ok = true;
            for(uint8_t k = 2; k < 6; k++) {
                if(!(a->rx_fdata[(si+k)/8] & (0x80 >> ((si+k)%8)))) { sync_ok = false; break; }
            }
            if(!sync_ok) continue;
            // Found sync at si. Differential Manchester starts at si+6
            uint16_t doff = si + 6;
            if(doff + 128 > bits) break; // need 64*2 symbols
            uint8_t raw[9]; memset(raw, 0, 9);
            uint8_t decoded_bits = 0;
            bool last = (a->rx_fdata[doff/8] >> (7-(doff%8))) & 1;
            for(uint16_t di = 0; di < 128 && decoded_bits < 72; di += 2) {
                uint16_t p0 = doff + di, p1 = doff + di + 1;
                bool b0 = (a->rx_fdata[p0/8] >> (7-(p0%8))) & 1;
                bool b1 = (a->rx_fdata[p1/8] >> (7-(p1%8))) & 1;
                // Differential Manchester: transition at start of bit period = 1, no transition = 0
                bool bit_val = (b0 != last);
                raw[decoded_bits/8] |= (bit_val ? (0x80 >> (decoded_bits%8)) : 0);
                decoded_bits++;
                last = b1;
            }
            if(decoded_bits < 64) continue;
            // CRC-8 check (poly 0x80, init 7) over bytes 0-7, check byte 8
            uint8_t crc = 7;
            for(int i = 0; i < 8; i++) {
                crc ^= raw[i];
                for(int b = 0; b < 8; b++) crc = (crc & 0x80) ? (crc << 1) ^ 0x80 : crc << 1;
            }
            if(decoded_bits >= 72 && crc != raw[8]) continue;
            // Valid Toyota TPMS! Extract fields
            uint32_t tid = ((uint32_t)raw[0]<<24)|((uint32_t)raw[1]<<16)|((uint32_t)raw[2]<<8)|raw[3];
            float psi = (float)(((raw[4]&0x7f)<<1)|(raw[5]>>7)) * 0.25f - 7.0f;
            int temp = (((raw[5]&0x7f)<<1)|(raw[6]>>7)) - 40;
            // Find or create TPMS device
            NRDev* d2 = NULL;
            for(uint8_t i = 0; i < a->dev_count; i++) {
                if(a->devs[i].proto == NRProtoTPMS && a->devs[i].dev_id == tid) { d2 = &a->devs[i]; break; }
            }
            if(!d2 && a->dev_count < NR_MAX_DEVICES) {
                d2 = &a->devs[a->dev_count++];
                memset(d2, 0, sizeof(NRDev));
                d2->proto = NRProtoTPMS;
                d2->dev_id = tid;
                d2->te = te;
                d2->freq = a->rx_freq;
                snprintf(d2->name, NR_MAX_NAME, "Car %08lX", (unsigned long)tid);
                d2->useful = true;
            }
            if(!d2) break;
            d2->hits++;
            d2->last_seen = a->tick;
            d2->rssi = a->rx_on ? (int8_t)subghz_devices_get_rssi(a->radio) : -127;
            d2->confirmed = true;
            snprintf(d2->sigs[0].label, 20, "%dC %.1fpsi", temp, (double)psi);
            d2->sig_count = 1;
            nr_update_date(d2);
            return; // handled
        }
    }

    NRProto p = nr_classify(te, bits, data, len);

    // Read RSSI while signal is fresh
    int8_t rssi = -127;
    if(a->rx_on) rssi = (int8_t)subghz_devices_get_rssi(a->radio);

    // Protocol lock: ignore non-matching
    if(a->lock_proto >= 0 && p != (NRProto)a->lock_proto) return;

    uint32_t did = nr_dev_id(p, data, len, te);

    // Filter noise: EV1527 addr 0, power-of-2, or all-ones pattern
    if(p == NRProtoEV1527 && (did == 0 || (did & (did - 1)) == 0 || ((did + 1) & did) == 0)) return;
    // Filter noise: PT2262 addr 0 only (0x02 is a real remote!)
    if(p == NRProtoPT2262 && did == 0) return;
    int8_t di = nr_find_dev(a, p, did);

    if(di >= 0) {
        NRDev* d = &a->devs[di];
        if((a->tick - d->last_seen) >= NR_HIT_COOLDOWN) {
            d->hits++; d->last_seen = a->tick; d->rssi = rssi;
            nr_update_date(d);
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
        // Autosave all protocols except Honeywell, OOK meter, and half-TE noise (too spammy)
        if(p != NRProtoHoneywell && did != 0xB109 && did != 0xB108) {
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
    nr_update_date(d);
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

static void nr_update_date(NRDev* d) {
    DateTime dt;
    furi_hal_rtc_get_datetime(&dt);
    if(dt.year >= 2026) {
        const char* months[] = {"","Jan","Feb","Mar","Apr","May","Jun",
                                "Jul","Aug","Sep","Oct","Nov","Dec"};
        snprintf(d->last_seen_date, 12, "%s %d", months[dt.month], dt.day);
    }
}

// RSSI bars: 4 levels (0 = never seen = all dots)
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
                    if(nr_is_sensor[a->devs[j].proto] && a->devs[j].useful && !nr_can_replay(&a->devs[j])) sc++;
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
               (a->devs[i].seeded && a->devs[i].confirmed && a->devs[i].last_seen >= a->session_start)) &&
               !(a->devs[i].proto == NRProtoKeeloq && !a->devs[i].seeded && a->devs[i].hits < 2)) live++;
        const char* fq = nr_freq_names[a->freq_mode];
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
            // KeeLoq: require 2+ hits to show (filters phantom fobs from encrypted alarm)
            if(d->proto == NRProtoKeeloq && !d->seeded && d->hits < 2) continue;
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
        canvas_draw_str(c, 0, FTR_Y, "OK:Detail LongOK:Save Bk");

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
            uint8_t max_vis = (FTR_LINE - (ROW_START + ROW_H + 10)) / ROW_H;
            uint8_t sstart = a->dev_scroll >= max_vis ? a->dev_scroll - max_vis + 1 : 0;
            for(uint8_t s = sstart; s < d->sig_count; s++) {
                uint8_t y = ROW_START + ROW_H + 10 + (s - sstart) * ROW_H;
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
        canvas_draw_str(c, 0, FTR_Y, "OK:SEND  U/D:Btn  L/R:Dev");

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
            snprintf(buf, sizeof(buf), "%c%s %s %lu%s %s",
                tag, nr_picon[d->proto], dname, (unsigned long)d->hits,
                nr_rssi_icon(d->rssi), when);
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

        // Auto-scroll to keep selected button visible (info=1 row + signals)
        if(nr_can_replay(d) && a->sig_sel + 2 > a->dev_scroll + MAX_ROWS) {
            a->dev_scroll = a->sig_sel + 2 - MAX_ROWS;
        }
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
                if(nr_can_replay(d) && s == a->sig_sel) {
                    canvas_draw_box(c, 0, y, 128, ROW_H);
                    canvas_set_color(c, ColorWhite);
                }
                snprintf(buf, sizeof(buf), " %s %s",
                    (nr_can_replay(d) && s == a->sig_sel) ? ">>" : " >", d->sigs[s].label);
                canvas_draw_str(c, 0, y + 8, buf);
                canvas_set_color(c, ColorBlack);
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
            canvas_draw_str(c, 0, FTR_Y, "OK:Send U/D:Btn L/R:Dev");
        else
            canvas_draw_str(c, 0, FTR_Y, "U/D:Scroll L/R:Dev Bk");

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
            if(nr_is_sensor[a->devs[i].proto] && a->devs[i].useful && !nr_can_replay(&a->devs[i])) si[sc++] = i;

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
                snprintf(buf, sizeof(buf), "%c~ %s %lu%s %s",
                    tag, d->sigs[0].label, (unsigned long)d->hits,
                    nr_rssi_icon(d->rssi), when);
            else if(d->proto == NRProtoHoneywell)
                snprintf(buf, sizeof(buf), "%c# %s %lu%s %s",
                    tag, d->name, (unsigned long)d->hits,
                    nr_rssi_icon(d->rssi), when);
            else
                snprintf(buf, sizeof(buf), "%c%s %s %lu%s %s",
                    tag, nr_picon[d->proto], d->name, (unsigned long)d->hits,
                    nr_rssi_icon(d->rssi), when);
            buf[42] = 0;
            canvas_draw_str(c, 0, y + 8, buf);
            canvas_set_color(c, ColorBlack);
        }
        if(sc == 0) canvas_draw_str(c, 10, 32, "No sensors found");

        canvas_draw_line(c, 0, FTR_LINE, 127, FTR_LINE);
        canvas_draw_str(c, 0, FTR_Y, "OK:Detail  Bk:Menu");

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
                    // Short OK: open device detail
                    int8_t ri = nr_live_idx(a, a->sel);
                    if(ri >= 0) {
                        a->dev_sel = ri;
                        a->dev_scroll = 0;
                        a->sig_sel = 0;
                        a->dev_back = NRViewScan;
                        a->view = NRViewDevice;
                    }
                } else if(ev.key == InputKeyOk && ev.type == InputTypeLong && lc > 0) {
                    // Long OK: mark device as useful (persists) + save DB
                    int8_t ri = nr_live_idx(a, a->sel);
                    if(ri >= 0) {
                        a->devs[ri].useful = true;
                        nr_save(a);
                        notification_message(a->notif, &sequence_blink_green_100);
                    }
                } else if(ev.key == InputKeyLeft || ev.key == InputKeyRight) {
                    // L/R: cycle frequency mode
                    if(ev.key == InputKeyRight) {
                        a->freq_mode = (a->freq_mode + 1) % NR_FREQ_COUNT;
                    } else {
                        a->freq_mode = a->freq_mode == 0 ? NR_FREQ_COUNT - 1 : a->freq_mode - 1;
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
                } else if(ev.key == InputKeyOk && ev.type == InputTypeShort && ri < a->dev_count) {
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
                        a->sig_sel = 0;
                        a->dev_back = NRViewKnown;
                        a->view = NRViewDevice;
                        nr_rx_start(a);
                    }
                }

            } else if(a->view == NRViewDevice) {
                NRDev* d = a->dev_sel < a->dev_count ? &a->devs[a->dev_sel] : NULL;
                if(ev.key == InputKeyBack) {
                    nr_rx_stop(a);
                    a->view = a->dev_back;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeShort &&
                          d && d->sig_count > 0 && nr_can_replay(d)) {
                    uint8_t si = a->sig_sel < d->sig_count ? a->sig_sel : 0;
                    notification_message(a->notif, &sequence_blink_magenta_100);
                    nr_tx(a, d, &d->sigs[si]);
                    notification_message(a->notif, &sequence_blink_green_100);
                    a->tx_flash = a->tick;
                } else if(ev.key == InputKeyUp) {
                    if(d && nr_can_replay(d) && a->sig_sel > 0)
                        a->sig_sel--;
                    else if(!nr_can_replay(d) && a->dev_scroll > 0)
                        a->dev_scroll--;
                } else if(ev.key == InputKeyDown) {
                    if(d && nr_can_replay(d) && a->sig_sel + 1 < d->sig_count)
                        a->sig_sel++;
                    else if(!nr_can_replay(d) && a->dev_scroll < 20)
                        a->dev_scroll++;
                } else if(ev.key == InputKeyLeft || ev.key == InputKeyRight) {
                    // L/R: cycle through devices from same source list
                    uint8_t li[NR_MAX_DEVICES], lc2 = 0;
                    if(a->dev_back == NRViewSensors) {
                        for(uint8_t i = 0; i < a->dev_count; i++)
                            if(nr_is_sensor[a->devs[i].proto] && a->devs[i].useful && !nr_can_replay(&a->devs[i])) li[lc2++] = i;
                    } else if(a->dev_back == NRViewScan) {
                        for(uint8_t i = 0; i < a->dev_count; i++)
                            if(a->devs[i].last_seen >= a->session_start) li[lc2++] = i;
                    } else {
                        for(uint8_t i = 0; i < a->dev_count; i++)
                            if(a->devs[i].useful) li[lc2++] = i;
                    }
                    // Find current position in list
                    uint8_t cur = 0;
                    for(uint8_t i = 0; i < lc2; i++)
                        if(li[i] == a->dev_sel) { cur = i; break; }
                    if(ev.key == InputKeyRight && cur + 1 < lc2) cur++;
                    else if(ev.key == InputKeyLeft && cur > 0) cur--;
                    a->dev_sel = li[cur];
                    a->dev_scroll = 0;
                    a->sig_sel = 0;
                }

            } else if(a->view == NRViewSensors) {
                // Count sensors
                uint8_t si[NR_MAX_DEVICES], sc = 0;
                for(uint8_t i = 0; i < a->dev_count; i++)
                    if(nr_is_sensor[a->devs[i].proto]) si[sc++] = i;

                if(ev.key == InputKeyBack) {
                    nr_rx_stop(a);
                    a->view = NRViewMenu;
                } else if(ev.key == InputKeyUp && a->sel > 0) {
                    a->sel--;
                } else if(ev.key == InputKeyDown && a->sel + 1 < sc) {
                    a->sel++;
                } else if(ev.key == InputKeyOk && ev.type == InputTypeShort && a->sel < sc) {
                    a->dev_sel = si[a->sel];
                    a->dev_scroll = 0;
                    a->sig_sel = 0;
                    a->dev_back = NRViewSensors;
                    a->view = NRViewDevice;
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
                        a->freq_mode = (a->freq_mode + 1) % NR_FREQ_COUNT;
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
