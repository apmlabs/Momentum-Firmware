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
#include <dialogs/dialogs.h>
#include <lib/flipper_format/flipper_format.h>
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/toolbox/level_duration.h>
#include <toolbox/path.h>
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

#define DOOYA_DIR       EXT_PATH("apps_data/dooya_remote")
#define DOOYA_EXT       ".dooya"
#define DOOYA_NAME_MAX  32

// Loaded remote data
typedef struct {
    uint32_t id;
    uint32_t addr;
    uint16_t cmd_up;
    uint16_t cmd_stop;
    uint16_t cmd_down;
    uint16_t cmd_confirm;
    char name[DOOYA_NAME_MAX];
    FuriString* file_path;
} DooyaRemoteData;

typedef enum { RxIdle, RxPreamble, RxSync, RxData } DooyaRxState;

typedef struct {
    ViewPort* view_port;
    Gui* gui;
    FuriMessageQueue* event_queue;
    NotificationApp* notifications;
    const SubGhzDevice* radio;
    SubGhzWorker* worker;
    bool running;

    // Current remote
    DooyaRemoteData remote;
    bool has_remote; // false = no file loaded

    // UI state
    bool transmitting;
    uint8_t last_cmd; // 1=up 2=stop 3=down
    uint16_t pending_cmd;
    bool pending_confirm;
    bool in_learn;
    uint8_t learn_btn; // 0=UP 1=STOP 2=DOWN 3=done

    // TX
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

    // Temp for blocking UI
    ViewDispatcher* vd;
    uint32_t menu_result;
    char name_buf[DOOYA_NAME_MAX];
} DooyaApp;
