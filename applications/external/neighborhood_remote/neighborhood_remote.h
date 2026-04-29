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
#define NR_MAX_NAME      24

typedef enum {
    NRProtoHoneywell,  // Manchester TE~143, serial+event
    NRProtoPT2262,     // PWM TE~194, addr+cmd — REPLAYABLE
    NRProtoEV1527,     // PWM TE~117, addr+cmd — REPLAYABLE
    NRProtoKeeloq,     // PWM TE~250, serial+btn (encrypted)
    NRProtoFSK,        // FSK on AM (high entropy)
    NRProtoBinRAW,     // Unknown
    NRProtoCount
} NRProto;

static const char* nr_proto_name[] = {
    "Honeywell", "PT2262", "EV1527", "Keeloq", "FSK?", "BinRAW"};

// > = replayable, # = listen-only (lock), ? = unknown
static const char* nr_proto_icon[] = {
    "#", ">", ">", "#", "?", "?"};
static const bool nr_proto_replayable[] = {
    false, true, true, false, false, false};

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
} NRSignal;

typedef enum { NRViewDash, NRViewScan, NRViewDetail } NRView;
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
    uint32_t              tick;

    NRView                view;
    NRSortMode            sort;

    // RX pulse decoder
    uint32_t              rx_pulse;     // last HIGH duration
    uint8_t               rx_bits[128];
    uint16_t              rx_bit_count;
    uint32_t              rx_te_sum;
    uint16_t              rx_te_n;
    volatile bool         rx_frame_ready;
    uint16_t              rx_frame_bits;
    uint16_t              rx_frame_te;
    uint8_t               rx_frame_data[32];
    uint8_t               rx_frame_len;

    uint16_t              autosave_seq;
} NRApp;
