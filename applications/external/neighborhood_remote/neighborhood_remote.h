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

#define NR_DIR           APP_DATA_PATH("neighborhood")
#define NR_SAVE_FILE     APP_DATA_PATH("neighborhood/devices.txt")
#define NR_MAX_DEVICES   16
#define NR_MAX_BUTTONS   8
#define NR_MAX_NAME      20

// Device categories matching our awareness skill
typedef enum {
    NRCatAlarm,      // Honeywell alarm sensors (TE~143, Manchester)
    NRCatRemote,     // Fixed-code remotes (TE~194, replayable)
    NRCatEV1527,     // EV1527/PT2262 sensors (TE~118, replayable)
    NRCatKeeloq,     // Rolling code (TE~250, listen only)
    NRCatWeather,    // Weather stations (TE~81)
    NRCatUnknown,    // BinRAW unknown
    NRCatCount
} NRCategory;

static const char* nr_cat_names[] = {"Alarm", "Remote", "EV1527", "Keeloq", "Weather", "Unknown"};

static const char* nr_cat_desc[] = {
    "Honeywell: serial, zone, open/close, tamper, battery",
    "Fixed-code remote (replayable, no encryption)",
    "EV1527: 20-bit addr + 4-bit cmd (replayable)",
    "Keeloq rolling code (listen only, encrypted)",
    "Weather station (temp, humidity, passive)",
    "Unknown BinRAW signal (capture & store)",
};

typedef struct {
    char     name[NR_MAX_NAME];
    uint16_t te;
    uint16_t bit_count;
    uint8_t  data[32];     // up to 256 bits
    uint8_t  data_len;     // bytes used
} NRButton;

typedef struct {
    char       name[NR_MAX_NAME];
    NRCategory category;
    uint8_t    btn_count;
    NRButton   buttons[NR_MAX_BUTTONS];
} NRDevice;

typedef enum {
    NRViewList,    // Device list
    NRViewDetail,  // Device detail + buttons
    NRViewLearn,   // Learning new signal
} NRView;

// RX state machine for raw pulse decoding
typedef enum {
    NRRxIdle,
    NRRxPreamble,
    NRRxData,
} NRRxState;

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
    uint8_t               sel_device;
    uint8_t               sel_button;

    NRView                current_view;

    // RX learn state
    NRRxState             rx_state;
    int32_t               rx_pulses[512];
    uint16_t              rx_pulse_count;
    volatile bool         rx_got_signal;
    uint32_t              rx_last_dur;

    // Decoded learn result
    uint16_t              learn_te;
    uint16_t              learn_bits;
    uint8_t               learn_data[32];
    uint8_t               learn_data_len;
    NRCategory            learn_cat;
    char                  learn_info[64]; // decoded info string
} NRApp;
