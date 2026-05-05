/*
 * Neighborhood v4 — 433.92 MHz device awareness & replay
 * Menu-first: Scan, My Remotes, Known Devices, Settings
 */
#include "neighborhood_remote.h"
#define TAG "Neighborhood"

// ============== Device ops ==============

static void nr_dev_label(NRDev* d) {
    if(d->name[0]) return;
    if(d->proto == NRProtoPT2262)
        snprintf(d->name, NR_MAX_NAME, "Remote %02X", (unsigned)(d->dev_id & 0xFF));
    else if(d->proto == NRProtoEV1527)
        snprintf(d->name, NR_MAX_NAME, "Sens %05lX", (unsigned long)(d->dev_id & 0xFFFFF));
    else if(d->proto == NRProtoKeeloq)
        snprintf(d->name, NR_MAX_NAME, "Fob %07lX", (unsigned long)(d->dev_id & 0xFFFFFFF));
    else if(d->proto == NRProtoHoneywell)
        snprintf(d->name, NR_MAX_NAME, "Alarm System");
    else if(d->proto == NRProtoFSK)
        snprintf(d->name, NR_MAX_NAME, "FSK Sensor");
    else if(d->proto == NRProtoNexusTH)
        snprintf(d->name, NR_MAX_NAME, "Weather %02X", (unsigned)(d->dev_id & 0xFF));
    else
        snprintf(d->name, NR_MAX_NAME, "Dev %04lX", (unsigned long)(d->dev_id & 0xFFFF));
}

static int8_t nr_find_dev(NRApp* a, NRProto p, uint32_t id) {
    for(uint8_t i = 0; i < a->dev_count; i++)
        if(a->devs[i].proto == p && a->devs[i].dev_id == id) return i;
    return -1;
}

// Can this device be replayed via firmware transmitter?
static bool nr_can_replay(NRDev* d) {
    if(d->sig_count == 0) return false;
    // Need at least one signal with a .sub file
    for(uint8_t i = 0; i < d->sig_count; i++)
        if(d->sigs[i].has_file) return true;
    return false;
}

// ============== Save/Load DB ==============

static void nr_save(NRApp* a) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, NR_SAVE_FILE)) {
        flipper_format_write_header_cstr(ff, "Neighborhood DB", 6);
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
            flipper_format_write_string_cstr(ff, "FWProto",
                d->fw_proto[0] ? d->fw_proto : "--");
            for(uint8_t s = 0; s < d->sig_count; s++) {
                flipper_format_write_string_cstr(ff, "SL", d->sigs[s].label);
                uint32_t sf[2] = {d->sigs[s].file_seq, d->sigs[s].has_file};
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
        if(flipper_format_read_header(ff, t, &ver) && ver >= 3 && ver <= 6) {
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
                if(ver >= 6 && flipper_format_read_string(ff, "FWProto", s)) {
                    const char* fp = furi_string_get_cstr(s);
                    if(strcmp(fp, "--") != 0)
                        snprintf(d->fw_proto, 16, "%s", fp);
                }
                for(uint8_t j = 0; j < sc; j++) {
                    if(!flipper_format_read_string(ff, "SL", s)) break;
                    snprintf(d->sigs[j].label, 20, "%s", furi_string_get_cstr(s));
                    if(ver >= 6) {
                        uint32_t sf[2] = {0, 0};
                        if(flipper_format_read_uint32(ff, "SF", sf, 2)) {
                            d->sigs[j].file_seq = sf[0];
                            d->sigs[j].has_file = sf[1];
                        }
                    } else {
                        // Old format: skip SB/SD fields
                        uint32_t sb[2] = {0, 0};
                        flipper_format_read_uint32(ff, "SB", sb, 2);
                        if(sb[1] > 0) {
                            uint8_t dd[32] = {0};
                            flipper_format_read_hex(ff, "SD", dd, sb[1] > 32 ? 32 : sb[1]);
                        }
                        d->sigs[j].has_file = false;
                    }
                    d->sig_count++;
                }
                d->useful = (d->proto != NRProtoBinRAW) || d->fw_proto[0];
                if(d->proto == NRProtoNexusTH && d->sig_count > 0 &&
                   strstr(d->sigs[0].label, "bad") != NULL) d->useful = false;
                a->dev_count++;
            }
            furi_string_free(s);
        }
        furi_string_free(t);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

// Write a .sub file for a seeded signal
static void nr_seed_sub(NRApp* a, const char* proto, uint32_t freq, uint16_t bits,
                        const char* key_hex, uint16_t te, uint16_t seq) {
    UNUSED(a);
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, NR_SAVE_DIR);
    storage_simply_mkdir(st, NR_AUTOSAVE_DIR);
    char path[80];
    snprintf(path, sizeof(path), "%s/%04d.sub", NR_AUTOSAVE_DIR, seq);
    File* file = storage_file_alloc(st);
    if(storage_file_open(file, path, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FuriString* s = furi_string_alloc();
        if(strcmp(proto, "BinRAW") == 0) {
            furi_string_printf(s,
                "Filetype: Flipper SubGhz Key File\nVersion: 1\n"
                "Frequency: %lu\nPreset: FuriHalSubGhzPresetOok650Async\n"
                "Protocol: BinRAW\nBit: %u\nTE: %u\nBit_RAW: %u\nData_RAW: %s\n",
                (unsigned long)freq, bits, te, bits, key_hex);
        } else {
            furi_string_printf(s,
                "Filetype: Flipper SubGhz Key File\nVersion: 1\n"
                "Frequency: %lu\nPreset: FuriHalSubGhzPresetOok650Async\n"
                "Protocol: %s\nBit: %u\nKey: %s\n",
                (unsigned long)freq, proto, bits, key_hex);
            if(te && strcmp(proto, "Princeton") == 0)
                furi_string_cat_printf(s, "TE: %u\nGuard_time: 31\n", te);
        }
        storage_file_write(file, furi_string_get_cstr(s), furi_string_size(s));
        furi_string_free(s);
    }
    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
}

static void nr_seed(NRApp* a) {
    if(a->dev_count > 0) return;
    #define SEED(P,TE,ID,HITS,NAME,DATE,FREQ,FWPROTO) { \
        NRDev* d = &a->devs[a->dev_count++]; memset(d,0,sizeof(NRDev)); \
        d->proto=P; d->te=TE; d->dev_id=ID; d->hits=HITS; d->seeded=true; \
        d->useful=true; d->freq=FREQ; snprintf(d->name, NR_MAX_NAME, NAME); \
        snprintf(d->last_seen_date, 12, DATE); if(FWPROTO[0]) snprintf(d->fw_proto, 16, "%s", FWPROTO); }

    SEED(NRProtoHoneywell, 143, 0x5800, 1633, "Alarm System", "May 2", 433920000, "Honeywell");
    SEED(NRProtoKeeloq, 322, 0x2F9AE15, 24, "Parking Fob", "Apr 27", 433920000, "KeeLoq");
    a->devs[a->dev_count-1].sig_count = 2;
    snprintf(a->devs[a->dev_count-1].sigs[0].label, 20, "S2 2F9AE1");
    snprintf(a->devs[a->dev_count-1].sigs[1].label, 20, "S3 2F9AE1");

    // Remote 4F — Princeton, 2 buttons with BinRAW .sub files for replay
    SEED(NRProtoPT2262, 194, 0x4F, 53, "Remote 4F", "May 2", 433920000, "Princeton");
    { NRDev* r = &a->devs[a->dev_count-1];
      snprintf(r->sigs[0].label, 20, "Btn A");
      r->sigs[0].file_seq = 9000; r->sigs[0].has_file = true;
      snprintf(r->sigs[1].label, 20, "Btn B");
      r->sigs[1].file_seq = 9001; r->sigs[1].has_file = true;
      r->sig_count = 2;
      nr_seed_sub(a, "BinRAW", 433920000, 192,
        "80 00 00 00 EE EE EE EE EE EE EE E8 8E 88 EE EE EE EE EE EE EE E8 88 88", 194, 9000);
      nr_seed_sub(a, "BinRAW", 433920000, 128,
        "80 00 00 00 88 88 88 88 8E 88 8E 88 E8 88 88 88", 194, 9001);
    }

    // Neighbor Gate — Princeton TE=311, 4 buttons (3 captured + 1 computed), strong RSSI
    SEED(NRProtoPT2262, 311, 0x87, 4, "Neighbor Gate", "May 5", 433920000, "Princeton");
    { NRDev* r = &a->devs[a->dev_count-1];
      snprintf(r->sigs[0].label, 20, "Open");
      r->sigs[0].file_seq = 9002; r->sigs[0].has_file = true;
      snprintf(r->sigs[1].label, 20, "Close");
      r->sigs[1].file_seq = 9003; r->sigs[1].has_file = true;
      snprintf(r->sigs[2].label, 20, "Pedestrian");
      r->sigs[2].file_seq = 9004; r->sigs[2].has_file = true;
      snprintf(r->sigs[3].label, 20, "Light");
      r->sigs[3].file_seq = 9005; r->sigs[3].has_file = true;
      r->sig_count = 4;
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 71", 311, 9002);
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 72", 311, 9003);
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 74", 311, 9004);
      nr_seed_sub(a, "Princeton", 433920000, 24, "00 00 00 00 00 9C B8 78", 311, 9005);
    }

    SEED(NRProtoFSK, 65, 0xF5C0, 118, "FSK Sensor", "Apr 30", 433920000, "");
    SEED(NRProtoBinRAW, 98, 0xB109, 699, "OOK Unknown 98", "May 2", 433920000, "BinRAW");
    SEED(NRProtoNexusTH, 650, 0xE0E0, 29, "Weather E0", "May 2", 433920000, "NexusTH");
    a->devs[a->dev_count-1].sig_count = 1;
    snprintf(a->devs[a->dev_count-1].sigs[0].label, 20, "16.5C");
    SEED(NRProtoBinRAW, 345, 0xB122, 10, "Bell Ctrl", "May 2", 433920000, "");

    // Garage — CAME 12-bit 868 MHz with .sub file
    SEED(NRProtoBinRAW, 320, 0x09EC, 19, "Garage", "May 4", 868350000, "CAME");
    { NRDev* g = &a->devs[a->dev_count-1];
      snprintf(g->sigs[0].label, 20, "CAME 0x9EC");
      g->sigs[0].file_seq = 9006; g->sigs[0].has_file = true;
      g->sig_count = 1;
      nr_seed_sub(a, "CAME", 868350000, 12, "00 00 00 00 00 00 09 EC", 0, 9006);
    }

    // Dooya Windows — 3 remotes, STOP command each, with .sub files
    SEED(NRProtoBinRAW, 366, 0xC0A16C, 0, "Window 1", "May 5", 433920000, "Dooya");
    { NRDev* d = &a->devs[a->dev_count-1];
      snprintf(d->sigs[0].label, 20, "UP"); d->sigs[0].file_seq = 9010; d->sigs[0].has_file = true;
      snprintf(d->sigs[1].label, 20, "STOP"); d->sigs[1].file_seq = 9011; d->sigs[1].has_file = true;
      snprintf(d->sigs[2].label, 20, "DOWN"); d->sigs[2].file_seq = 9012; d->sigs[2].has_file = true;
      d->sig_count = 3;
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 A1 6C 01 00 0B D9", 0, 9010);
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 A1 6C 01 00 23 F1", 0, 9011);
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 A1 6C 01 00 43 11", 0, 9012);
    }
    SEED(NRProtoBinRAW, 366, 0xC0AD01, 0, "Window 2", "May 5", 433920000, "Dooya");
    { NRDev* d = &a->devs[a->dev_count-1];
      snprintf(d->sigs[0].label, 20, "UP"); d->sigs[0].file_seq = 9020; d->sigs[0].has_file = true;
      snprintf(d->sigs[1].label, 20, "STOP"); d->sigs[1].file_seq = 9021; d->sigs[1].has_file = true;
      snprintf(d->sigs[2].label, 20, "DOWN"); d->sigs[2].file_seq = 9022; d->sigs[2].has_file = true;
      d->sig_count = 3;
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 AD 01 01 00 0B 7A", 0, 9020);
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 AD 01 01 00 23 92", 0, 9021);
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 AD 01 01 00 43 B2", 0, 9022);
    }
    SEED(NRProtoBinRAW, 366, 0xC09EBD, 0, "Window 3", "May 5", 433920000, "Dooya");
    { NRDev* d = &a->devs[a->dev_count-1];
      snprintf(d->sigs[0].label, 20, "UP"); d->sigs[0].file_seq = 9030; d->sigs[0].has_file = true;
      snprintf(d->sigs[1].label, 20, "STOP"); d->sigs[1].file_seq = 9031; d->sigs[1].has_file = true;
      snprintf(d->sigs[2].label, 20, "DOWN"); d->sigs[2].file_seq = 9032; d->sigs[2].has_file = true;
      d->sig_count = 3;
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 9E BD 01 00 0B 27", 0, 9030);
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 9E BD 01 00 23 3F", 0, 9031);
      nr_seed_sub(a, "Dooya", 433920000, 64, "A3 C0 9E BD 01 00 43 5F", 0, 9032);
    }
    #undef SEED
    a->autosave_seq = 100; // start live captures at 100 to avoid seed file conflicts
}

// ============== RX / Radio / TX ==============

// Map firmware protocol name to our NRProto enum
static NRProto nr_map_proto(const char* name) {
    if(strcmp(name, "Princeton") == 0) return NRProtoPT2262;
    if(strcmp(name, "KeeLoq") == 0) return NRProtoKeeloq;
    if(strcmp(name, "Honeywell") == 0 || strcmp(name, "Honeywell_WDB") == 0) return NRProtoHoneywell;
    if(strstr(name, "Nexus") != NULL) return NRProtoNexusTH;
    if(strcmp(name, "SMC5326") == 0 || strcmp(name, "GateTX") == 0 ||
       strcmp(name, "Linear") == 0 || strcmp(name, "NiceFlo") == 0) return NRProtoEV1527;
    return NRProtoBinRAW;
}

// Extract device ID from firmware decode string
static uint32_t nr_extract_id(const char* proto, const char* ds, uint16_t* te_out) {
    if(strcmp(proto, "Princeton") == 0) {
        char* kp = strstr(ds, "Key:0x");
        char* tp = strstr(ds, "TE:");
        if(te_out && tp) *te_out = atoi(tp + 3);
        if(kp) { uint32_t k = strtoul(kp + 6, NULL, 16); return (k >> 4) & 0xFF; }
    } else if(strcmp(proto, "KeeLoq") == 0) {
        char* sp = strstr(ds, "Sn:0x");
        if(te_out) *te_out = 300;
        if(sp) return strtoul(sp + 5, NULL, 16) & 0xFFFFFFF;
    } else if(strcmp(proto, "Honeywell") == 0 || strcmp(proto, "Honeywell_WDB") == 0) {
        if(te_out) *te_out = 143;
        return 0x5800;
    } else if(strstr(proto, "Nexus") != NULL) {
        char* ip = strstr(ds, "Id:");
        if(te_out) *te_out = 500;
        if(ip) return 0xE000 | (strtoul(ip + 3, NULL, 16) & 0xFF);
        return 0xE000;
    } else if(strncmp(proto, "CAME", 4) == 0) {
        char* kp = strstr(ds, "Key:0x");
        if(te_out) *te_out = 320;
        if(kp) return strtoul(kp + 6, NULL, 16) & 0xFFFF;
    } else if(strcmp(proto, "Dooya") == 0) {
        char* kp = strstr(ds, "Key:0x");
        if(te_out) *te_out = 366;
        if(kp) { uint64_t k = strtoull(kp + 6, NULL, 16); return (k >> 24) & 0xFFFFFF; }
    } else if(strcmp(proto, "BinRAW") == 0) {
        char* tp = strstr(ds, "TE:");
        uint16_t te = tp ? atoi(tp + 3) : 100;
        if(te_out) *te_out = te;
        if(te >= 90 && te <= 109) return 0xB109;
        return 0xB100 | ((te / 10) & 0xFF);
    }
    if(te_out) *te_out = 200;
    return 0xB100;
}

// Extract signal label from firmware decode string
static void nr_extract_label(const char* proto, const char* ds, char* out, uint8_t sz) {
    if(strcmp(proto, "Princeton") == 0) {
        char* kp = strstr(ds, "Key:0x");
        if(kp) { uint32_t k = strtoul(kp + 6, NULL, 16); snprintf(out, sz, "Cmd:%02X", (unsigned)(k & 0xFF)); return; }
    } else if(strcmp(proto, "KeeLoq") == 0) {
        char* sp = strstr(ds, "Sn:0x");
        char* bp = strstr(ds, "Btn:0x");
        if(sp) { uint32_t sn = strtoul(sp + 5, NULL, 16); uint8_t btn = bp ? strtoul(bp + 6, NULL, 16) : 0;
            snprintf(out, sz, "S%u %06lX", btn, (unsigned long)(sn & 0xFFFFFFF)); return; }
    } else if(strncmp(proto, "CAME", 4) == 0) {
        char* kp = strstr(ds, "Key:0x");
        if(kp) { snprintf(out, sz, "CAME 0x%lX", (unsigned long)strtoul(kp + 6, NULL, 16)); return; }
    } else if(strcmp(proto, "Dooya") == 0) {
        char* kp = strstr(ds, "Key:0x");
        if(kp) { snprintf(out, sz, "Dooya %s", kp + 6); out[sz-1] = 0;
            char* nl = strchr(out, '\r'); if(nl) *nl = 0; return; }
    } else if(strstr(proto, "Nexus") != NULL) {
        char* tp = strstr(ds, "Temp:");
        if(tp) { snprintf(out, sz, "%.15s", tp + 5); char* nl = strchr(out, '\r'); if(nl) *nl = 0; return; }
    } else if(strcmp(proto, "Honeywell") == 0) {
        snprintf(out, sz, "event"); return;
    }
    snprintf(out, sz, "%.15s", proto);
}

// Firmware protocol decode callback — PRIMARY signal handler
// Runs in worker thread. Does device management + autosave.
static void nr_decode_cb(SubGhzReceiver* rx, SubGhzProtocolDecoderBase* db, void* ctx) {
    UNUSED(rx);
    NRApp* a = ctx;

    // Dedup: same hash within 600ms = skip
    uint32_t hash = subghz_protocol_decoder_base_get_hash_data_long(db);
    uint32_t now = furi_get_tick();
    if(hash == a->last_decode_hash && (now - a->last_decode_tick) < 600) {
        a->last_decode_tick = now;
        return;
    }
    a->last_decode_hash = hash;
    a->last_decode_tick = now;

    const char* pn = db->protocol->name;
    NRProto proto = nr_map_proto(pn);

    // Get decode string for field extraction
    FuriString* text = furi_string_alloc();
    subghz_protocol_decoder_base_get_string(db, text);
    const char* ds = furi_string_get_cstr(text);

    uint16_t te = 0;
    uint32_t dev_id = nr_extract_id(pn, ds, &te);
    char label[20] = {0};
    nr_extract_label(pn, ds, label, 20);

    // Protocol lock filter
    if(a->lock_proto >= 0 && proto != (NRProto)a->lock_proto) { furi_string_free(text); return; }

    // Find or create device
    int8_t di = nr_find_dev(a, proto, dev_id);
    // CAME special: match garage seed
    if(di < 0 && proto == NRProtoBinRAW && strncmp(pn, "CAME", 4) == 0) {
        int8_t gi = nr_find_dev(a, NRProtoBinRAW, 0x09EC);
        if(gi >= 0 && dev_id == 0x09EC) di = gi;
    }

    if(di >= 0) {
        NRDev* d = &a->devs[di];
        if((a->tick - d->last_seen) >= NR_HIT_COOLDOWN) d->hits++;
        d->last_seen = a->tick;
        if(d->seeded) d->confirmed = true;
        if(!d->fw_proto[0]) snprintf(d->fw_proto, 16, "%s", pn);
        // Update first signal label
        if(label[0] && d->sig_count > 0) snprintf(d->sigs[0].label, 20, "%s", label);
        // For replayable: store unique signals
        if(nr_can_replay(d) && label[0] && d->sig_count < NR_MAX_SIGS) {
            bool found = false;
            for(uint8_t i = 0; i < d->sig_count; i++)
                if(strcmp(d->sigs[i].label, label) == 0) { found = true; break; }
            if(!found) {
                NRSig* ns = &d->sigs[d->sig_count++];
                memset(ns, 0, sizeof(NRSig));
                snprintf(ns->label, 20, "%s", label);
            }
        }
    } else if(a->dev_count < NR_MAX_DEVICES) {
        NRDev* d = &a->devs[a->dev_count];
        memset(d, 0, sizeof(NRDev));
        d->proto = proto; d->te = te; d->dev_id = dev_id;
        d->freq = a->rx_freq; d->hits = 1; d->last_seen = a->tick;
        d->useful = (proto != NRProtoBinRAW);
        snprintf(d->fw_proto, 16, "%s", pn);
        if(label[0]) { snprintf(d->sigs[0].label, 20, "%s", label); d->sig_count = 1; }
        nr_dev_label(d);
        di = a->dev_count++;
        notification_message(a->notif, &sequence_blink_cyan_10);
    }

    // Autosave: serialize proper .sub file (skip Honeywell spam)
    if(di >= 0 && a->autosave && proto != NRProtoHoneywell) {
        Storage* st = furi_record_open(RECORD_STORAGE);
        storage_simply_mkdir(st, NR_SAVE_DIR);
        storage_simply_mkdir(st, NR_AUTOSAVE_DIR);
        char path[80];
        snprintf(path, sizeof(path), "%s/%04d.sub", NR_AUTOSAVE_DIR, a->autosave_seq);
        FlipperFormat* ff = flipper_format_file_alloc(st);
        if(flipper_format_file_open_always(ff, path)) {
            SubGhzRadioPreset preset = {
                .frequency = a->rx_freq,
                .name = furi_string_alloc_set("AM650"),
                .data = NULL, .data_size = 0,
            };
            subghz_protocol_decoder_base_serialize(db, ff, &preset);
            furi_string_free(preset.name);
            // Mark signal as having a file for replay
            NRDev* d = &a->devs[di];
            if(d->sig_count > 0) {
                // Update last signal's file reference
                uint8_t si = d->sig_count - 1;
                d->sigs[si].file_seq = a->autosave_seq;
                d->sigs[si].has_file = true;
            }
        }
        flipper_format_free(ff);
        a->autosave_seq++;
        furi_record_close(RECORD_STORAGE);
    }

    furi_string_free(text);
    a->rx_new_signal = true;
}

// RX callback — just feeds firmware decoders
static void nr_rx_cb(void* ctx, bool level, uint32_t duration) {
    NRApp* a = ctx;
    subghz_receiver_decode(a->receiver, level, duration);
}

static void nr_rx_start(NRApp* a) {
    if(a->rx_on) return;
    subghz_devices_idle(a->radio);
    subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
    uint32_t freq = (a->freq_mode == NRFreq868) ? 868350000 : 433920000;
    a->rx_freq = freq;
    subghz_devices_set_frequency(a->radio, freq);
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

// TX: replay signal from saved .sub file using firmware transmitter
static void nr_tx(NRApp* a, NRDev* d, NRSig* s) {
    if(!s->has_file) return;
    bool was = a->rx_on; if(was) nr_rx_stop(a);

    char path[80];
    snprintf(path, sizeof(path), "%s/%04d.sub", NR_AUTOSAVE_DIR, s->file_seq);

    Storage* st = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_existing(ff, path)) {
        FuriString* proto_name = furi_string_alloc();
        if(flipper_format_read_string(ff, "Protocol", proto_name)) {
            flipper_format_rewind(ff);
            SubGhzTransmitter* transmitter = subghz_transmitter_alloc_init(
                a->environment, furi_string_get_cstr(proto_name));
            if(transmitter) {
                if(subghz_transmitter_deserialize(transmitter, ff) == SubGhzProtocolStatusOk) {
                    subghz_devices_idle(a->radio);
                    subghz_devices_load_preset(a->radio, FuriHalSubGhzPresetOok650Async, NULL);
                    subghz_devices_set_frequency(a->radio, d->freq ? d->freq : 433920000);
                    if(subghz_devices_start_async_tx(a->radio, subghz_transmitter_yield, transmitter)) {
                        while(!subghz_devices_is_async_complete_tx(a->radio))
                            furi_delay_ms(10);
                        subghz_devices_stop_async_tx(a->radio);
                    }
                }
                subghz_transmitter_free(transmitter);
            }
        }
        furi_string_free(proto_name);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
    subghz_devices_idle(a->radio);
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

static void nr_process(NRApp* a) { UNUSED(a); }

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
            // Signal list with scroll (3 visible rows below device name)
            uint8_t sig_rows = 3; // rows available for signals
            uint8_t sig_start = 0;
            if(a->dev_scroll >= sig_rows) sig_start = a->dev_scroll - sig_rows + 1;
            for(uint8_t s = sig_start; s < d->sig_count && (s - sig_start) < sig_rows; s++) {
                uint8_t y = ROW_START + ROW_H + 2 + (s - sig_start) * ROW_H;
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

        // For replayable devices, auto-scroll to keep selected signal visible
        // Info line takes 1 row, so signals start at row 1. MAX_ROWS=4 means rows 0-3 visible.
        // Signal s is at row (1 + s). Visible if (1 + s - scroll) < MAX_ROWS && (1 + s - scroll) >= 0
        uint8_t dev_scroll_offset = 0;
        if(nr_can_replay(d) && d->sig_count > 0) {
            uint8_t sel_row = 1 + a->dev_scroll; // row of selected signal (0-based from info line)
            if(sel_row >= MAX_ROWS) dev_scroll_offset = sel_row - MAX_ROWS + 1;
        }
        int8_t line = nr_can_replay(d) ? -(int8_t)dev_scroll_offset : -(int8_t)a->dev_scroll;
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
                bool sel = nr_can_replay(d) && (s == a->dev_scroll);
                if(sel) {
                    canvas_draw_box(c, 0, y, 128, ROW_H);
                    canvas_set_color(c, ColorWhite);
                }
                snprintf(buf, sizeof(buf), " %s %s",
                    (d->sigs[s].has_file) ? ">" : " ", d->sigs[s].label);
                canvas_draw_str(c, 0, y + 8, buf);
                if(sel) canvas_set_color(c, ColorBlack);
            }
            line++;
        }
        if(line >= 0 && line < MAX_ROWS) {
            uint8_t y = ROW_START + line * ROW_H + 4;
            canvas_draw_line(c, 0, y, 127, y);
        }
        line++;
        const char* desc = d->fw_proto[0] ? d->fw_proto : nr_pdesc[d->proto];
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
        if(nr_can_replay(d) && d->sig_count > 1)
            canvas_draw_str(c, 0, FTR_Y, "OK:Send U/D:Btn Bk");
        else if(nr_can_replay(d))
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
                    a->lock_proto = -1;
                    nr_rx_stop(a);
                    // Return to Sensors if device is a sensor, else Known Devices
                    a->view = (d && nr_is_sensor[d->proto] && !nr_can_replay(d)) ? NRViewSensors : NRViewKnown;
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
                } else if(ev.key == InputKeyUp) {
                    if(nr_can_replay(d) && d->sig_count > 1) {
                        // Cycle signal selection for replayable devices
                        a->dev_scroll = a->dev_scroll > 0 ? a->dev_scroll - 1 : d->sig_count - 1;
                    } else if(a->dev_scroll > 0) {
                        a->dev_scroll--;
                    }
                } else if(ev.key == InputKeyDown) {
                    if(nr_can_replay(d) && d->sig_count > 1) {
                        a->dev_scroll = (a->dev_scroll + 1) % d->sig_count;
                    } else if(a->dev_scroll < 20) {
                        a->dev_scroll++;
                    }
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
                    if(nr_is_sensor[a->devs[i].proto] && !nr_can_replay(&a->devs[i])) si[sc++] = i;

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
