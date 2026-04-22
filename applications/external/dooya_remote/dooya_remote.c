/*
 * Dooya Remote — Motorized blind/curtain controller
 *
 * Protocol (reverse-engineered from captures + ESPHome dooya_protocol.cpp):
 *   Freq:     433.92 MHz OOK
 *   Preamble: 8 × (290us HIGH + 600us LOW)
 *   Sync:     5000us HIGH + 650us LOW
 *   Data:     64 bits PWM — bit 1: 600H+290L, bit 0: 290H+600L
 *   Frame:    [24-bit ID][24-bit addr][16-bit cmd]
 *   TX:       UP/DOWN: 6× cmd + 6× confirm, STOP: 6× cmd
 *   Gap:      ~5000us between frames
 */

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <lib/subghz/devices/devices.h>
#include <lib/toolbox/level_duration.h>
#include "helpers/radio_device_loader.h"

#define TAG "DooyaRemote"

#define DOOYA_SHORT   290
#define DOOYA_LONG    600
#define DOOYA_SYNC_H  5000
#define DOOYA_SYNC_L  650
#define DOOYA_GAP     5000
#define DOOYA_PRE     8
#define DOOYA_BITS    64
#define DOOYA_REPEATS 6

// Frame: preamble(8*2) + sync(2) + data(64*2) + gap(2) = 148 LevelDurations
// Max 12 frames (6 cmd + 6 confirm) = 1776
#define DOOYA_UPLOAD_MAX 1800

#define DOOYA_ADDR_HI  0xA3C0A1ULL
#define DOOYA_ADDR_LO  0x6C0100ULL
#define DOOYA_CMD_UP      0x0BD9
#define DOOYA_CMD_DOWN    0x4311
#define DOOYA_CMD_STOP    0x23F1
#define DOOYA_CMD_CONFIRM 0x24F2

typedef struct {
    ViewPort* view_port;
    Gui* gui;
    FuriMessageQueue* event_queue;
    NotificationApp* notifications;
    const SubGhzDevice* radio;
    bool running;
    uint8_t last_cmd; // 0=none 1=up 2=stop 3=down
    bool transmitting;
    // TX state (accessed from ISR)
    LevelDuration* upload;
    volatile uint16_t upload_size;
    volatile uint16_t upload_idx;
} DooyaApp;

static uint16_t dooya_encode_frame(LevelDuration* buf, uint16_t pos, uint64_t data) {
    // Preamble
    for(uint8_t i = 0; i < DOOYA_PRE; i++) {
        buf[pos++] = level_duration_make(true, DOOYA_SHORT);
        buf[pos++] = level_duration_make(false, DOOYA_LONG);
    }
    // Sync
    buf[pos++] = level_duration_make(true, DOOYA_SYNC_H);
    buf[pos++] = level_duration_make(false, DOOYA_SYNC_L);
    // 64 data bits MSB first
    for(int8_t bit = DOOYA_BITS - 1; bit >= 0; bit--) {
        if((data >> bit) & 1) {
            buf[pos++] = level_duration_make(true, DOOYA_LONG);
            buf[pos++] = level_duration_make(false, DOOYA_SHORT);
        } else {
            buf[pos++] = level_duration_make(true, DOOYA_SHORT);
            buf[pos++] = level_duration_make(false, DOOYA_LONG);
        }
    }
    // Gap
    buf[pos++] = level_duration_make(true, DOOYA_SHORT);
    buf[pos++] = level_duration_make(false, DOOYA_GAP);
    return pos;
}

// Async TX callback — called from ISR context by CC1101 driver
static LevelDuration dooya_tx_yield(void* ctx) {
    DooyaApp* app = ctx;
    if(app->upload_idx >= app->upload_size) {
        return level_duration_reset();
    }
    return app->upload[app->upload_idx++];
}

static void dooya_transmit(DooyaApp* app, uint16_t cmd, bool with_confirm) {
    app->transmitting = true;
    view_port_update(app->view_port);

    uint64_t data = (DOOYA_ADDR_HI << 40) | (DOOYA_ADDR_LO << 16) | cmd;
    uint64_t confirm = (DOOYA_ADDR_HI << 40) | (DOOYA_ADDR_LO << 16) | DOOYA_CMD_CONFIRM;

    // Build upload buffer
    uint16_t pos = 0;
    for(uint8_t r = 0; r < DOOYA_REPEATS; r++)
        pos = dooya_encode_frame(app->upload, pos, data);
    if(with_confirm) {
        for(uint8_t r = 0; r < DOOYA_REPEATS; r++)
            pos = dooya_encode_frame(app->upload, pos, confirm);
    }
    app->upload_size = pos;
    app->upload_idx = 0;

    // TX
    subghz_devices_idle(app->radio);
    subghz_devices_load_preset(app->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio, 433920000);
    subghz_devices_set_async_mirror_pin(app->radio, NULL);

    if(subghz_devices_start_async_tx(app->radio, dooya_tx_yield, app)) {
        while(!subghz_devices_is_async_complete_tx(app->radio)) {
            furi_delay_ms(10);
        }
        subghz_devices_stop_async_tx(app->radio);
    }

    subghz_devices_idle(app->radio);
    app->transmitting = false;
    view_port_update(app->view_port);
}

static void dooya_draw_cb(Canvas* canvas, void* ctx) {
    DooyaApp* app = ctx;
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 28, 12, "Dooya Remote");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 6, 24, "ID:A3C0A1 Addr:6C0100");

    // UP button (top center)
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_rframe(canvas, 40, 28, 48, 14, 2);
    if(app->last_cmd == 1) canvas_draw_rbox(canvas, 40, 28, 48, 14, 2);
    if(app->last_cmd == 1) canvas_set_color(canvas, ColorWhite);
    canvas_draw_str(canvas, 52, 39, "UP ^");
    canvas_set_color(canvas, ColorBlack);

    // STOP button (bottom left)
    canvas_draw_rframe(canvas, 2, 46, 42, 14, 2);
    if(app->last_cmd == 2) canvas_draw_rbox(canvas, 2, 46, 42, 14, 2);
    if(app->last_cmd == 2) canvas_set_color(canvas, ColorWhite);
    canvas_draw_str(canvas, 7, 57, "STOP");
    canvas_set_color(canvas, ColorBlack);

    // DOWN button (bottom right)
    canvas_draw_rframe(canvas, 84, 46, 42, 14, 2);
    if(app->last_cmd == 3) canvas_draw_rbox(canvas, 84, 46, 42, 14, 2);
    if(app->last_cmd == 3) canvas_set_color(canvas, ColorWhite);
    canvas_draw_str(canvas, 89, 57, "DN v");
    canvas_set_color(canvas, ColorBlack);

    if(app->transmitting) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 48, 57, "TX...");
    }
}

static void dooya_input_cb(InputEvent* ev, void* ctx) {
    furi_message_queue_put(((DooyaApp*)ctx)->event_queue, ev, FuriWaitForever);
}

int32_t dooya_remote_app(void* p) {
    UNUSED(p);
    DooyaApp* app = malloc(sizeof(DooyaApp));
    memset(app, 0, sizeof(DooyaApp));
    app->running = true;
    app->upload = malloc(DOOYA_UPLOAD_MAX * sizeof(LevelDuration));

    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    subghz_devices_init();
    app->radio = radio_device_loader_set(NULL, SubGhzRadioDeviceTypeInternal);

    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, dooya_draw_cb, app);
    view_port_input_callback_set(app->view_port, dooya_input_cb, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    InputEvent event;
    while(app->running) {
        if(furi_message_queue_get(app->event_queue, &event, FuriWaitForever) == FuriStatusOk) {
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                app->running = false;
            } else if(event.type == InputTypeShort && !app->transmitting) {
                uint16_t cmd = 0;
                bool confirm = false;
                switch(event.key) {
                case InputKeyUp:    cmd = DOOYA_CMD_UP;   confirm = true; app->last_cmd = 1; break;
                case InputKeyOk:    cmd = DOOYA_CMD_STOP; confirm = false; app->last_cmd = 2; break;
                case InputKeyDown:  cmd = DOOYA_CMD_DOWN; confirm = true; app->last_cmd = 3; break;
                default: break;
                }
                if(cmd) {
                    dooya_transmit(app, cmd, confirm);
                    notification_message(app->notifications, &sequence_blink_cyan_100);
                }
            }
        }
    }

    subghz_devices_idle(app->radio);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);
    radio_device_loader_end(app->radio);
    subghz_devices_deinit();
    furi_record_close(RECORD_NOTIFICATION);
    furi_message_queue_free(app->event_queue);
    free(app->upload);
    free(app);
    return 0;
}
