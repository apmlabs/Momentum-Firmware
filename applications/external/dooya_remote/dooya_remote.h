#pragma once
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/view_dispatcher.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>
#include <lib/flipper_format/flipper_format.h>
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/toolbox/level_duration.h>
#include "helpers/radio_device_loader.h"

#define DOOYA_SHORT      290
#define DOOYA_LONG       600
#define DOOYA_SYNC_H     5000
#define DOOYA_SYNC_L     650
#define DOOYA_GAP        5000
#define DOOYA_PRE        8
#define DOOYA_BITS       64
#define DOOYA_REPEATS    3
#define DOOYA_UPLOAD_MAX 900

#define DOOYA_DIR        "/ext/apps_data/dooya_remote"
#define DOOYA_SAVE_FILE  DOOYA_DIR "/remotes.txt"
#define DOOYA_NAME_LEN   16
#define DOOYA_MAX_BTNS   8
#define DOOYA_MAX_REMOTES 4

// A single learned button
typedef struct {
    char name[DOOYA_NAME_LEN];
    uint64_t frame;
} DooyaButton;

// A remote: ID + addr + list of buttons
typedef struct {
    uint32_t id;
    uint32_t addr;
    DooyaButton buttons[DOOYA_MAX_BTNS];
    uint8_t btn_count;
    char name[DOOYA_NAME_LEN];
} DooyaRemoteData;

typedef enum { DooyaModeRemote, DooyaModeLearn, DooyaModeScan } DooyaMode;
typedef enum { RxIdle, RxPreamble, RxSync, RxData } DooyaRxState;

typedef struct {
    ViewPort* view_port;
    Gui* gui;
    FuriMessageQueue* event_queue;
    NotificationApp* notifications;
    const SubGhzDevice* radio;
    SubGhzWorker* worker;
    bool running;

    DooyaMode mode;
    DooyaRemoteData remotes[DOOYA_MAX_REMOTES];
    uint8_t remote_count;
    uint8_t remote_sel;
    uint8_t btn_sel;       // selected button index in current remote
    uint8_t learn_start;   // btn_count when learn mode started

    bool transmitting;
    uint16_t pending_cmd;
    LevelDuration* upload;
    volatile uint16_t upload_size;
    volatile uint16_t upload_idx;

    // RX decoder (ISR context)
    volatile DooyaRxState rx_state;
    volatile uint8_t rx_pre_count;
    volatile uint8_t rx_bit_count;
    volatile uint64_t rx_data;
    volatile bool rx_frame_ready;
    volatile uint64_t rx_frame;

    // Blocking UI
    ViewDispatcher* vd;
    uint32_t menu_result;
    char name_buf[DOOYA_NAME_LEN];

    // Scan mode
    uint16_t scan_id16;     // bytes 1+2 of remote ID (0x0000-0xFFFF), byte 0 fixed at 0xC0
    uint8_t scan_ch;        // current channel 1-16
    uint8_t scan_btn;       // 0=UP, 1=DOWN, 2=STOP
    bool scan_running;      // auto-advance active
} DooyaApp;
