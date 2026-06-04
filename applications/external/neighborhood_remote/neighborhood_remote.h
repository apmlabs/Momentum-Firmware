#pragma once
#include <furi.h>
#include <gui/gui.h>
#include <gui/view_port.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/subghz/receiver.h>
#include <lib/subghz/environment.h>
#include <lib/subghz/protocols/protocol_items.h>
#include <lib/subghz/protocols/base.h>
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/devices/cc1101_int/cc1101_int_interconnect.h>
#include <storage/storage.h>
#include <flipper_format/flipper_format.h>

#define NR_SAVE_DIR      APP_DATA_PATH("neighborhood")
#define NR_SAVE_FILE     APP_DATA_PATH("neighborhood/devices.txt")
#define NR_AUTOSAVE_DIR  APP_DATA_PATH("neighborhood/autosave")
#define NR_MAX_DEVICES   64
#define NR_MAX_SIGS      4
#define NR_MAX_NAME      16
#define NR_HIT_COOLDOWN  40

typedef enum {
    NRProtoHoneywell, NRProtoPT2262, NRProtoEV1527,
    NRProtoKeeloq, NRProtoFSK, NRProtoNexusTH, NRProtoTPMS, NRProtoBinRAW, NRProtoCount
} NRProto;

static const char* nr_pname[] = {"Honeywell","PT2262","EV1527","Keeloq","FSK","NexusTH","TPMS","BinRAW"};
static const char* nr_picon[] = {"#",">",">","#","?","~","C","?"};
static const bool nr_replayable[] = {false,true,true,false,false,false,false,false};
static const bool nr_is_sensor[] = {true,false,false,false,true,true,true,true};
static const char* nr_pdesc[] = {
    "Honeywell-family 5800EU alarm.\nManchester TE=143us 64-bit.\nFFFE+ch+serial+event+CRC.\nEvent: open tamper alarm\nbattery heartbeat.\n12+ zones. NOT replayable.",
    "PT2262/Princeton remote.\nPWM TE=194us 24-bit.\nAddress + command.\nGarage, doorbell, switch.\nFixed code. REPLAYABLE.",
    "EV1527 fixed OTP code.\nPWM TE=117us 24-bit.\n20-bit addr + 4-bit cmd.\nF=PIR E=Door 8=Panic\n2=BtnB 4=BtnC.\nFixed code. REPLAYABLE.",
    "Keeloq HCS301 rolling.\nPWM TE=250us 66-bit.\n32-bit hop + serial + btn.\nEncrypted counter.\nNOT replayable.",
    "FSK on AM demodulator.\nRecapture on FM476.\nLikely weather or HVAC\nsensor. TE=65us typical.",
    "Nexus-TH weather sensor.\nOOK_PWM TE=650us 36-bit.\nID+flags+temp+humidity.\nAuriol/Lidl/Rubicson.\nUpdates every 50 seconds.",
    "Schrader TPMS tire sensor.\nManchester TE=120us 64-bit.\nID+pressure+temperature.\nCar passing nearby.\nBurst when wheel spinning.",
    "Unknown OOK protocol.\nRaw pulse timing only.\nTE and bit count shown.\nCheck TE for device type.",
};

typedef struct {
    uint8_t  raw[32];
    uint8_t  raw_len;
    uint16_t bits;
    char     label[20];
    uint16_t file_seq;  // .sub file sequence number (0=none)
    bool     has_file;  // true if .sub file exists for replay
    uint64_t tx_key;    // protocol key/frame for direct TX encoding
} NRSig;

typedef struct {
    NRProto  proto;
    uint16_t te;
    uint32_t dev_id;
    uint32_t freq;
    uint32_t hits;
    uint32_t last_seen;
    int8_t   rssi;
    char     name[NR_MAX_NAME];
    char     last_seen_date[12];
    char     fw_proto[16];  // firmware decoder protocol name (annotation)
    NRSig    sigs[NR_MAX_SIGS];
    uint8_t  sig_count;
    bool     seeded;
    bool     confirmed;
    bool     saved;
    bool     useful;
} NRDev;

typedef enum {
    NRViewMenu, NRViewScan, NRViewRemotes,
    NRViewKnown, NRViewDevice, NRViewSensors, NRViewSettings,
    NRViewCameScan
} NRView;

typedef enum { NRSortHits, NRSortRecent } NRSort;
typedef enum { NRFreq433AM, NRFreq433FM, NRFreq868AM, NRFreq868FM } NRFreqMode;
#define NR_FREQ_COUNT 4

static const uint32_t nr_freq_vals[] = {433920000, 433920000, 868350000, 868350000};
static const char* nr_freq_names[] = {"433 AM", "433 FM", "868 AM", "868 FM"};

typedef struct {
    Gui* gui;
    ViewPort* vp;
    FuriMessageQueue* eq;
    NotificationApp* notif;
    const SubGhzDevice* radio;
    SubGhzWorker* worker;
    SubGhzEnvironment* environment;
    SubGhzReceiver* receiver;
    bool rx_on;

    NRDev    devs[NR_MAX_DEVICES];
    uint8_t  dev_count;
    uint8_t  sel;
    uint8_t  dev_sel;
    uint8_t  dev_scroll;
    uint8_t  sig_sel;
    NRView   dev_back; // view to return to on Back from Device
    uint8_t  dev_source; // 0=known, 1=sensors, 2=scan
    uint8_t  menu_sel;
    uint32_t tick;
    uint32_t session_start;
    uint8_t  scan_anim;

    NRView   view;
    NRSort   sort;
    int8_t   lock_proto;
    bool     autosave;
    NRFreqMode freq_mode;
    uint32_t rx_freq;

    // RX double buffer (bit accumulator)
    uint32_t rx_pulse;
    uint8_t  rx_bits[128];
    uint16_t rx_bit_count;
    uint32_t rx_te_sum;
    uint16_t rx_te_n;
    volatile bool rx_ready;
    uint16_t rx_fbits;
    uint16_t rx_fte;
    uint8_t  rx_fdata[32];
    uint8_t  rx_flen;
    uint8_t  rx_last[32];
    uint8_t  rx_last_len;
    uint16_t rx_last_te;
    uint32_t tx_flash;
    uint16_t autosave_seq;

    // CAME scan state
    uint16_t came_code;
    bool     came_running;
    bool     came_tx;

    // Firmware protocol decoder result (lock-free handoff)
    volatile bool dec_ready;
    char dec_proto[16];
    char dec_str[128];

    // Dooya A-OK 64-bit RX state machine
    uint8_t  dooya_state;  // 0=idle 1=pre 2=sync 3=data
    uint8_t  dooya_pre;
    uint8_t  dooya_bits;
    uint64_t dooya_data;

    // NexusTH state machine decoder
    uint8_t  nexus_state; // 0=wait_sync 1=save_pulse 2=check_gap
    uint8_t  nexus_bits;
    uint32_t nexus_pulse;
    uint64_t nexus_data;
    uint32_t dooya_last_hash;

    // Schrader TPMS Manchester decoder
    uint8_t  tpms_state;   // 0=idle, 1=preamble, 2=data
    uint8_t  tpms_pre;     // preamble bit counter
    uint8_t  tpms_bit_cnt;
    uint64_t tpms_data;
    uint8_t  tpms_manch;   // manchester state
    bool     tpms_last_level;
    uint32_t tpms_last_dur;

    // Direct TX upload buffer (same method as Dooya Remote app)
    LevelDuration upload[900];
    volatile uint16_t upload_size;
    volatile uint16_t upload_idx;
} NRApp;
