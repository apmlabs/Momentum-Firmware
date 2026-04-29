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
#define NR_MAX_SIGNALS   32
#define NR_MAX_NAME      16
#define NR_HIT_COOLDOWN  40  // ticks (~2s at 50ms/tick) between counting same device

typedef enum {
    NRProtoHoneywell,  // # Manchester TE~143
    NRProtoPT2262,     // > PWM TE~194 — REPLAYABLE
    NRProtoEV1527,     // > PWM TE~117 — REPLAYABLE
    NRProtoKeeloq,     // # PWM TE~250 (encrypted)
    NRProtoFSK,        // ? FSK on AM
    NRProtoBinRAW,     // ? Unknown
    NRProtoCount
} NRProto;

static const char* nr_proto_name[] = {
    "Honeywell", "PT2262", "EV1527", "Keeloq", "FSK", "BinRAW"};
static const char* nr_proto_icon[] = {
    "#", ">", ">", "#", "?", "?"};
static const bool nr_proto_replayable[] = {
    false, true, true, false, false, false};

// Protocol encyclopedia text (stored in flash)
static const char* nr_proto_desc[] = {
    "Honeywell 5800 alarm\nManchester TE=143us\n64-bit: FFFE+serial+\nevent+CRC\nEvents: open tamper\nlobat alarm heartbeat\nNOT replayable",
    "PT2262/Princeton\nPWM encoding TE=194us\n24-bit: address+command\nCommon in remotes,\ndoorbells, switches\nREPLAYABLE",
    "EV1527 learning code\nPWM encoding TE=117us\n25-bit: 20-bit addr +\n4-bit command\nCommon in remotes,\nsensors, alarms\nREPLAYABLE",
    "Keeloq HCS301\nPWM encoding TE=250us\n66-bit: 32-bit hop +\n28-bit serial + btn\nRolling code encrypted\nNOT replayable",
    "FSK modulated signal\nCaptured on AM = noise\nNeed FM476 to decode\nLikely weather sensor\nor building system",
    "Unknown protocol\nBinRAW capture only\nTE and bit count shown\nMay decode with more\ncaptures or SDR",
};

typedef struct {
    NRProto  proto;
    uint16_t te;
    uint16_t bit_count;
    uint32_t hits;
    uint32_t last_seen;   // tick of last hit
    uint8_t  data[8];     // decoded payload
    char     info[40];    // human-readable decode
    uint8_t  raw_frame[32];
    uint8_t  raw_len;
    char     name[NR_MAX_NAME]; // user-assigned name
} NRSignal;

typedef enum {
    NRViewDash,
    NRViewScan,
    NRViewDetail,
    NRViewInfo,
    NRViewLibrary,
} NRView;

typedef enum { NRSortHits, NRSortRecent } NRSortMode;

typedef struct {
    Gui*                  gui;
    ViewPort*             view_port;
    FuriMessageQueue*     event_queue;
    NotificationApp*      notifications;

    const SubGhzDevice*   radio_device;
    SubGhzWorker*         worker;
    bool                  rx_active;

    NRSignal              signals[NR_MAX_SIGNALS];
    uint8_t               signal_count;
    uint8_t               sel;
    uint8_t               lib_sel;       // library view selection
    uint32_t              tick;
    uint8_t               scan_anim;     // scan animation frame

    NRView                view;
    NRSortMode            sort;
    int8_t                filter;        // -1=all, 0..5=specific proto

    // RX pulse decoder — double buffered
    uint32_t              rx_pulse;
    uint8_t               rx_bits[128];
    uint16_t              rx_bit_count;
    uint32_t              rx_te_sum;
    uint16_t              rx_te_n;

    // Buffer A (ISR writes here)
    volatile bool         rx_frame_ready;
    uint16_t              rx_frame_bits;
    uint16_t              rx_frame_te;
    uint8_t               rx_frame_data[32];
    uint8_t               rx_frame_len;

    // Buffer B (main loop copies here for processing)
    uint16_t              proc_bits;
    uint16_t              proc_te;
    uint8_t               proc_data[32];
    uint8_t               proc_len;

    uint16_t              autosave_seq;
    uint8_t               info_scroll;   // scroll position in info view
} NRApp;
