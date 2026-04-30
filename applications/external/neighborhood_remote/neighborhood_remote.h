#pragma once
#include <furi.h>
#include <gui/gui.h>
#include <gui/view_port.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <lib/subghz/subghz_worker.h>
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
    uint8_t  raw[32];
    uint8_t  raw_len;
    uint16_t bits;
    char     label[20];
} NRSig;

typedef struct {
    NRProto  proto;
    uint16_t te;
    uint32_t dev_id;
    uint32_t hits;
    uint32_t last_seen;
    int8_t   rssi;      // last RSSI in dBm
    char     name[NR_MAX_NAME];
    NRSig    sigs[NR_MAX_SIGS];
    uint8_t  sig_count;
    bool     seeded;
    bool     confirmed;
    bool     saved;
} NRDev;

typedef enum {
    NRViewMenu, NRViewScan, NRViewRemotes,
    NRViewKnown, NRViewDevice, NRViewSensors, NRViewSettings
} NRView;

typedef enum { NRSortHits, NRSortRecent } NRSort;

typedef struct {
    Gui* gui;
    ViewPort* vp;
    FuriMessageQueue* eq;
    NotificationApp* notif;
    const SubGhzDevice* radio;
    SubGhzWorker* worker;
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

    // RX double buffer
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
} NRApp;
