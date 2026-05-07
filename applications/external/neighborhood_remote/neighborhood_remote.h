#pragma once
#include <furi.h>
#include <gui/gui.h>
#include <gui/view_port.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/subghz/receiver.h>
#include <lib/subghz/transmitter.h>
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
#define NR_MAX_DEVICES   18
#define NR_MAX_SIGS      4
#define NR_MAX_NAME      16
#define NR_HIT_COOLDOWN  40

typedef enum {
    NRProtoHoneywell, NRProtoPT2262, NRProtoEV1527,
    NRProtoKeeloq, NRProtoFSK, NRProtoNexusTH, NRProtoBinRAW, NRProtoCount
} NRProto;

static const char* nr_pname[] = {"Honeywell","PT2262","EV1527","Keeloq","FSK","NexusTH","BinRAW"};
static const char* nr_picon[] = {"#",">",">","#","?","~","?"};
static const bool nr_replayable[] = {false,true,true,false,false,false,false};
static const bool nr_is_sensor[] = {true,false,false,false,true,true,true};
static const char* nr_pdesc[] = {
    "Honeywell-family 5800EU alarm.\nManchester TE=143us 64-bit.\nFFFE+ch+serial+event+CRC.\nEvent: open tamper alarm\nbattery heartbeat.\n12+ zones. NOT replayable.",
    "PT2262/Princeton remote.\nPWM TE=194us 24-bit.\nAddress + command.\nGarage, doorbell, switch.\nFixed code. REPLAYABLE.",
    "EV1527 fixed OTP code.\nPWM TE=117us 24-bit.\n20-bit addr + 4-bit cmd.\nF=PIR E=Door 8=Panic\n2=BtnB 4=BtnC.\nFixed code. REPLAYABLE.",
    "Keeloq HCS301 rolling.\nPWM TE=250us 66-bit.\n32-bit hop + serial + btn.\nEncrypted counter.\nNOT replayable.",
    "FSK on AM demodulator.\nRecapture on FM476.\nLikely weather or HVAC\nsensor. TE=65us typical.",
    "Nexus-TH weather sensor.\nOOK_PWM TE=650us 36-bit.\nID+flags+temp+humidity.\nAuriol/Lidl/Rubicson.\nUpdates every 50 seconds.",
    "Unknown OOK protocol.\nRaw pulse timing only.\nTE and bit count shown.\nCheck TE for device type.",
};

typedef struct {
    char     label[20];
    uint16_t file_seq;   // autosave sequence number for .sub file replay
    bool     has_file;   // true if .sub file exists on SD
} NRSig;

typedef struct {
    NRProto  proto;
    uint16_t te;
    uint32_t dev_id;
    uint32_t freq;      // 433920000 or 868350000
    uint32_t hits;
    uint32_t last_seen;
    int8_t   rssi;
    char     name[NR_MAX_NAME];
    char     last_seen_date[12];
    char     fw_proto[16]; // firmware protocol name (for TX)
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
typedef enum { NRFreq433, NRFreq868, NRFreqAuto } NRFreqMode;

static const uint32_t nr_freq_vals[] = {433920000, 868350000};
static const char* nr_freq_names[] = {"433.92", "868.35", "Auto"};

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
    uint32_t auto_switch;

    // RX state (decode callback)
    volatile bool rx_new_signal;
    uint32_t last_decode_hash;
    uint32_t last_decode_tick;
    uint32_t tx_flash;
    uint16_t autosave_seq;

    // CAME scan state
    uint16_t came_code;
    bool     came_running;
    bool     came_tx;

    // Debug counters
    volatile uint32_t dbg_rx_cb;    // nr_rx_cb call count
    volatile uint32_t dbg_decode_cb; // nr_decode_cb call count
    volatile uint32_t dbg_overrun;   // stream buffer overrun count
    char dbg_last_proto[16];         // last protocol name that fired
} NRApp;
