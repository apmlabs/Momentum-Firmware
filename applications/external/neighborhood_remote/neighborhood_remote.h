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
#define NR_MAX_DEVICES   16
#define NR_MAX_SIGNALS   4   // per device (buttons/events)
#define NR_MAX_NAME      16
#define NR_HIT_COOLDOWN  40  // ~2s at 50ms/tick

typedef enum {
    NRProtoHoneywell,  // #
    NRProtoPT2262,     // >
    NRProtoEV1527,     // >
    NRProtoKeeloq,     // #
    NRProtoFSK,        // ?
    NRProtoBinRAW,     // ?
    NRProtoCount
} NRProto;

static const char* nr_proto_name[] = {
    "Honeywell", "PT2262", "EV1527", "Keeloq", "FSK", "BinRAW"};
static const char* nr_proto_icon[] = {
    "#", ">", ">", "#", "?", "?"};
static const bool nr_proto_replayable[] = {
    false, true, true, false, false, false};
static const char* nr_proto_desc[] = {
    "Honeywell 5800 alarm sensor.\n"
    "Manchester encoding, TE=143us.\n"
    "64-bit frame: FFFE preamble +\n"
    "4-bit channel + 20-bit serial\n"
    "+ 8-bit event + 16-bit CRC.\n"
    "Events: open, tamper, low bat,\n"
    "alarm, heartbeat.\n"
    "NOT replayable (CRC protected).",

    "PT2262 / Princeton remote.\n"
    "PWM encoding, TE=194us.\n"
    "24-bit frame: address + command.\n"
    "Common in garage remotes,\n"
    "doorbells, light switches.\n"
    "Fixed code — REPLAYABLE.",

    "EV1527 learning code remote.\n"
    "PWM encoding, TE=117us.\n"
    "25-bit: 20-bit address +\n"
    "4-bit command.\n"
    "Common in remotes, sensors,\n"
    "alarm keypads.\n"
    "Fixed code — REPLAYABLE.",

    "Keeloq HCS301 rolling code.\n"
    "PWM encoding, TE=250us.\n"
    "66-bit: 32-bit encrypted hop +\n"
    "28-bit serial + 4-bit button +\n"
    "2-bit status.\n"
    "Encrypted rolling code.\n"
    "NOT replayable.",

    "FSK modulated signal captured\n"
    "on AM modulation = garbage.\n"
    "Need to recapture on FM476\n"
    "for proper decode.\n"
    "Likely weather sensor or\n"
    "building automation system.",

    "Unknown protocol.\n"
    "BinRAW capture — raw pulse\n"
    "timing only, no decode.\n"
    "TE and bit count shown.\n"
    "May identify with more\n"
    "captures or SDR analysis.",
};

// A signal = one button press or event type
typedef struct {
    uint8_t  raw_frame[32];
    uint8_t  raw_len;
    uint16_t bit_count;
    char     label[20];    // "Button A", "Open", etc.
} NRSignalEntry;

// A device = one physical remote/sensor with multiple signals
typedef struct {
    NRProto        proto;
    uint16_t       te;
    uint32_t       device_id;    // address/serial for grouping
    uint32_t       hits;
    uint32_t       last_seen;
    char           name[NR_MAX_NAME];
    NRSignalEntry  sigs[NR_MAX_SIGNALS];
    uint8_t        sig_count;
} NRDevice;

typedef enum { NRViewDash, NRViewScan, NRViewDevice } NRView;
typedef enum { NRSortHits, NRSortRecent } NRSortMode;

typedef struct {
    Gui*                  gui;
    ViewPort*             view_port;
    FuriMessageQueue*     event_queue;
    NotificationApp*      notifications;

    const SubGhzDevice*   radio_device;
    SubGhzWorker*         worker;
    bool                  rx_active;

    NRDevice              devices[NR_MAX_DEVICES];
    uint8_t               device_count;
    uint8_t               sel;          // scan list selection
    uint8_t               dev_sel;      // device view: which device
    uint8_t               sig_sel;      // device view: which signal/line
    uint8_t               dev_scroll;   // device view: scroll offset
    uint32_t              tick;
    uint8_t               scan_anim;

    NRView                view;
    NRSortMode            sort;
    int8_t                filter;       // -1=all, 0..5=proto

    // RX double buffer
    uint32_t              rx_pulse;
    uint8_t               rx_bits[128];
    uint16_t              rx_bit_count;
    uint32_t              rx_te_sum;
    uint16_t              rx_te_n;
    volatile bool         rx_frame_ready;
    uint16_t              rx_frame_bits;
    uint16_t              rx_frame_te;
    uint8_t               rx_frame_data[32];
    uint8_t               rx_frame_len;
    uint16_t              proc_bits;
    uint16_t              proc_te;
    uint8_t               proc_data[32];
    uint8_t               proc_len;

    uint16_t              autosave_seq;
} NRApp;
