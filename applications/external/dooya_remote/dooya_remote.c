/*
 * Dooya Remote — Motorized blind/curtain controller with learning
 *
 * Protocol (reverse-engineered):
 *   Freq:     433.92 MHz OOK (AM650)
 *   Preamble: 8 × (290us HIGH + 600us LOW) — first frame only
 *   Sync:     5000us HIGH + 650us LOW
 *   Data:     64 bits PWM — bit 1: 600H+290L, bit 0: 290H+600L
 *   Frame:    [24-bit ID][24-bit addr][16-bit cmd]
 *   TX:       UP/DOWN: N× cmd + N× confirm, STOP: N× cmd
 */

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>
#include <lib/flipper_format/flipper_format.h>
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/toolbox/level_duration.h>
#include "helpers/radio_device_loader.h"

#define TAG "DooyaRemote"

// Protocol timing (microseconds)
#define DOOYA_SHORT   290
#define DOOYA_LONG    600
#define DOOYA_SYNC_H  5000
#define DOOYA_SYNC_L  650
#define DOOYA_GAP     5000
#define DOOYA_PRE     8
#define DOOYA_BITS    64
#define DOOYA_REPEATS 3

#define DOOYA_UPLOAD_MAX 900
#define DOOYA_CMD_CONFIRM 0x24F2
#define DOOYA_MAX_REMOTES 4
#define DOOYA_SAVE_PATH "/ext/apps_data/dooya_remote"
#define DOOYA_SAVE_FILE "/ext/apps_data/dooya_remote/remotes.txt"

// Learned remote: ID + addr + 3 commands
typedef struct {
    uint32_t id;    // 24-bit remote ID
    uint32_t addr;  // 24-bit address
    uint16_t cmd_up;
    uint16_t cmd_stop;
    uint16_t cmd_down;
    uint16_t cmd_confirm;
    char name[16];
} DooyaRemoteData;

typedef enum {
    DooyaModeRemote,  // normal remote control
    DooyaModeLearn,   // listening for signal
} DooyaMode;

// RX decoder state machine
typedef enum {
    RxIdle,
    RxPreamble,
    RxSync,
    RxData,
} DooyaRxState;

typedef struct {
    ViewPort* view_port;
    Gui* gui;
    FuriMessageQueue* event_queue;
    NotificationApp* notifications;
    const SubGhzDevice* radio;
    SubGhzWorker* worker;
    bool running;

    // Mode
    DooyaMode mode;
    uint8_t learn_btn; // 0=waiting for UP, 1=STOP, 2=DOWN, 3=done

    // Remotes
    DooyaRemoteData remotes[DOOYA_MAX_REMOTES];
    uint8_t remote_count;
    uint8_t remote_sel; // currently selected

    // TX state
    uint8_t last_cmd; // 0=none 1=up 2=stop 3=down
    bool transmitting;
    uint16_t pending_cmd;
    bool pending_confirm;
    LevelDuration* upload;
    volatile uint16_t upload_size;
    volatile uint16_t upload_idx;

    // RX decoder state (accessed from worker callback)
    volatile DooyaRxState rx_state;
    volatile uint8_t rx_pre_count;
    volatile uint8_t rx_bit_count;
    volatile uint64_t rx_data;
    volatile bool rx_frame_ready;
    volatile uint64_t rx_frame; // completed frame
} DooyaApp;

// ============== File I/O ==============
static void dooya_save(DooyaApp* app) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, DOOYA_SAVE_PATH);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, DOOYA_SAVE_FILE)) {
        flipper_format_write_header_cstr(ff, "Dooya Remotes", 1);
        uint32_t cnt = app->remote_count;
        flipper_format_write_uint32(ff, "Count", &cnt, 1);
        for(uint8_t i = 0; i < app->remote_count; i++) {
            DooyaRemoteData* r = &app->remotes[i];
            uint32_t vals[6] = {r->id, r->addr, r->cmd_up, r->cmd_stop, r->cmd_down, r->cmd_confirm};
            char key[8];
            snprintf(key, sizeof(key), "R%d", i);
            flipper_format_write_uint32(ff, key, vals, 6);
            char nkey[8];
            snprintf(nkey, sizeof(nkey), "N%d", i);
            flipper_format_write_string_cstr(ff, nkey, r->name);
        }
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
}

static void dooya_load(DooyaApp* app) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    app->remote_count = 0;
    if(flipper_format_file_open_existing(ff, DOOYA_SAVE_FILE)) {
        uint32_t ver = 0;
        FuriString* type = furi_string_alloc();
        if(flipper_format_read_header(ff, type, &ver)) {
            uint32_t cnt = 0;
            flipper_format_read_uint32(ff, "Count", &cnt, 1);
            if(cnt > DOOYA_MAX_REMOTES) cnt = DOOYA_MAX_REMOTES;
            for(uint32_t i = 0; i < cnt; i++) {
                uint32_t vals[6] = {0};
                char key[8];
                snprintf(key, sizeof(key), "R%d", (int)i);
                if(flipper_format_read_uint32(ff, key, vals, 6)) {
                    DooyaRemoteData* r = &app->remotes[i];
                    r->id = vals[0]; r->addr = vals[1];
                    r->cmd_up = vals[2]; r->cmd_stop = vals[3];
                    r->cmd_down = vals[4]; r->cmd_confirm = vals[5];
                    char nkey[8];
                    snprintf(nkey, sizeof(nkey), "N%d", (int)i);
                    FuriString* ns = furi_string_alloc();
                    if(flipper_format_read_string(ff, nkey, ns)) {
                        snprintf(r->name, sizeof(r->name), "%s", furi_string_get_cstr(ns));
                    } else {
                        snprintf(r->name, sizeof(r->name), "Rem %u", (unsigned)(i + 1));
                    }
                    furi_string_free(ns);
                    app->remote_count++;
                }
            }
        }
        furi_string_free(type);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);

    // If no saved remotes, create default
    if(app->remote_count == 0) {
        DooyaRemoteData* r = &app->remotes[0];
        r->id = 0xA3C0A1; r->addr = 0x6C0100;
        r->cmd_up = 0x0BD9; r->cmd_stop = 0x23F1;
        r->cmd_down = 0x4311; r->cmd_confirm = 0x24F2;
        snprintf(r->name, sizeof(r->name), "Default");
        app->remote_count = 1;
    }
}

// ============== TX ==============
static uint16_t dooya_encode_frame(LevelDuration* buf, uint16_t pos, uint64_t data) {
    for(uint8_t i = 0; i < DOOYA_PRE; i++) {
        buf[pos++] = level_duration_make(true, DOOYA_SHORT);
        buf[pos++] = level_duration_make(false, DOOYA_LONG);
    }
    buf[pos++] = level_duration_make(true, DOOYA_SYNC_H);
    buf[pos++] = level_duration_make(false, DOOYA_SYNC_L);
    for(int8_t bit = DOOYA_BITS - 1; bit >= 0; bit--) {
        if((data >> bit) & 1) {
            buf[pos++] = level_duration_make(true, DOOYA_LONG);
            buf[pos++] = level_duration_make(false, DOOYA_SHORT);
        } else {
            buf[pos++] = level_duration_make(true, DOOYA_SHORT);
            buf[pos++] = level_duration_make(false, DOOYA_LONG);
        }
    }
    buf[pos++] = level_duration_make(true, DOOYA_SHORT);
    buf[pos++] = level_duration_make(false, DOOYA_GAP);
    return pos;
}

static LevelDuration dooya_tx_yield(void* ctx) {
    DooyaApp* app = ctx;
    if(app->upload_idx >= app->upload_size) return level_duration_reset();
    return app->upload[app->upload_idx++];
}

static void dooya_transmit(DooyaApp* app, uint16_t cmd, bool with_confirm) {
    DooyaRemoteData* r = &app->remotes[app->remote_sel];
    app->transmitting = true;
    view_port_update(app->view_port);

    uint64_t data = ((uint64_t)r->id << 40) | ((uint64_t)r->addr << 16) | cmd;
    uint64_t conf = ((uint64_t)r->id << 40) | ((uint64_t)r->addr << 16) | r->cmd_confirm;

    uint16_t pos = 0;
    for(uint8_t i = 0; i < DOOYA_REPEATS; i++)
        pos = dooya_encode_frame(app->upload, pos, data);
    if(with_confirm) {
        for(uint8_t i = 0; i < DOOYA_REPEATS; i++)
            pos = dooya_encode_frame(app->upload, pos, conf);
    }
    app->upload_size = pos;
    app->upload_idx = 0;

    subghz_devices_idle(app->radio);
    subghz_devices_load_preset(app->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio, 433920000);
    subghz_devices_set_async_mirror_pin(app->radio, NULL);

    if(subghz_devices_start_async_tx(app->radio, dooya_tx_yield, app)) {
        while(!subghz_devices_is_async_complete_tx(app->radio)) furi_delay_ms(10);
        subghz_devices_stop_async_tx(app->radio);
    }
    subghz_devices_idle(app->radio);
    app->transmitting = false;
    view_port_update(app->view_port);
}

// ============== RX Decoder ==============
static void dooya_rx_callback(void* ctx, bool level, uint32_t duration) {
    DooyaApp* app = ctx;

    switch(app->rx_state) {
    case RxIdle:
        // Look for preamble: short HIGH
        if(level && duration > 180 && duration < 450) {
            app->rx_pre_count = 1;
            app->rx_state = RxPreamble;
        }
        break;

    case RxPreamble:
        if(!level && duration > 400 && duration < 800) {
            // Long LOW after short HIGH — preamble pair OK, wait for next
        } else if(level && duration > 180 && duration < 450) {
            app->rx_pre_count++;
        } else if(level && duration > 3500 && duration < 6500) {
            // Sync HIGH! Need at least 4 preamble pulses
            if(app->rx_pre_count >= 4) {
                app->rx_state = RxSync;
            } else {
                app->rx_state = RxIdle;
            }
        } else {
            app->rx_state = RxIdle;
        }
        break;

    case RxSync:
        // Expect sync LOW (~650us)
        if(!level && duration > 300 && duration < 1000) {
            app->rx_bit_count = 0;
            app->rx_data = 0;
            app->rx_state = RxData;
        } else {
            app->rx_state = RxIdle;
        }
        break;

    case RxData:
        if(level) {
            // HIGH pulse: long=1, short=0
            app->rx_data <<= 1;
            if(duration > 400) app->rx_data |= 1;
            app->rx_bit_count++;
            if(app->rx_bit_count >= 64) {
                app->rx_frame = app->rx_data;
                app->rx_frame_ready = true;
                app->rx_state = RxIdle;
            }
        } else if(duration > 3500) {
            // Gap or next sync — frame ended early
            if(app->rx_bit_count >= 48) {
                app->rx_frame = app->rx_data << (64 - app->rx_bit_count);
                app->rx_frame_ready = true;
            }
            app->rx_state = RxIdle;
        }
        // Normal LOW between bits — just continue
        break;
    }
}

static void dooya_rx_start(DooyaApp* app) {
    app->rx_state = RxIdle;
    app->rx_frame_ready = false;
    subghz_devices_idle(app->radio);
    subghz_devices_load_preset(app->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio, 433920000);
    subghz_worker_set_pair_callback(app->worker, (SubGhzWorkerPairCallback)dooya_rx_callback);
    subghz_worker_set_context(app->worker, app);
    subghz_devices_start_async_rx(app->radio, subghz_worker_rx_callback, app->worker);
    subghz_worker_start(app->worker);
}

static void dooya_rx_stop(DooyaApp* app) {
    subghz_worker_stop(app->worker);
    subghz_devices_stop_async_rx(app->radio);
    subghz_devices_idle(app->radio);
}

// ============== Drawing ==============
static void dooya_draw_remote(Canvas* canvas, DooyaApp* app) {
    DooyaRemoteData* r = &app->remotes[app->remote_sel];
    char buf[32];

    // Row 1: name
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 0, AlignCenter, AlignTop, r->name);

    // Row 2: status
    canvas_set_font(canvas, FontSecondary);
    if(app->transmitting) {
        canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignTop, ">>> Transmitting <<<");
    } else {
        snprintf(buf, sizeof(buf), "%06lX:%06lX  %d/%d",
            r->id, r->addr, app->remote_sel + 1, app->remote_count);
        canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignTop, buf);
    }

    // 3 buttons: y=21,32,43 — 10px tall, 1px gap
    static const uint8_t by[] = {21, 32, 43};
    static const char* labels[] = {"\x18 OPEN", "STOP", "\x19 CLOSE"};
    for(uint8_t i = 0; i < 3; i++) {
        canvas_draw_rframe(canvas, 30, by[i], 68, 10, 3);
        if(app->last_cmd == (i + 1) && app->transmitting) {
            canvas_draw_rbox(canvas, 30, by[i], 68, 10, 3);
            canvas_set_color(canvas, ColorWhite);
        }
        canvas_draw_str_aligned(canvas, 64, by[i] + 1, AlignCenter, AlignTop, labels[i]);
        canvas_set_color(canvas, ColorBlack);
    }

    // L/R arrows centered vertically with middle button
    canvas_draw_str_aligned(canvas, 15, 37, AlignCenter, AlignCenter, "<");
    canvas_draw_str_aligned(canvas, 113, 37, AlignCenter, AlignCenter, ">");

    // Bottom hint line — y=55 gives 8px clearance from buttons ending at y=53
    canvas_draw_str(canvas, 0, 63, "<Prev");
    canvas_draw_str_aligned(canvas, 64, 55, AlignCenter, AlignTop, "Hold OK:Menu");
    canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, "Next>");
}

static const char* learn_btn_names[] = {"UP", "STOP", "DOWN"};

static void dooya_draw_learn(Canvas* canvas, DooyaApp* app) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 2, AlignCenter, AlignTop, "Learn Remote");

    canvas_set_font(canvas, FontSecondary);
    if(app->learn_btn < 3) {
        char buf[40];
        snprintf(buf, sizeof(buf), "Press %s on the remote...", learn_btn_names[app->learn_btn]);
        canvas_draw_str_aligned(canvas, 64, 24, AlignCenter, AlignTop, buf);
        canvas_draw_str_aligned(canvas, 64, 36, AlignCenter, AlignTop, "Listening on 433.92 MHz");

        // Show what we've captured so far
        if(app->learn_btn > 0) {
            uint8_t idx = app->remote_count < DOOYA_MAX_REMOTES ? app->remote_count : DOOYA_MAX_REMOTES - 1;
            DooyaRemoteData* r = &app->remotes[idx];
            snprintf(buf, sizeof(buf), "ID:%06lX Addr:%06lX", r->id, r->addr);
            canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignTop, buf);
        }
    } else {
        canvas_draw_str_aligned(canvas, 64, 24, AlignCenter, AlignTop, "All buttons learned!");
        uint8_t idx = app->remote_count < DOOYA_MAX_REMOTES ? app->remote_count : DOOYA_MAX_REMOTES - 1;
        DooyaRemoteData* r = &app->remotes[idx];
        char buf[40];
        snprintf(buf, sizeof(buf), "ID:%06lX Addr:%06lX", r->id, r->addr);
        canvas_draw_str_aligned(canvas, 64, 36, AlignCenter, AlignTop, buf);
        snprintf(buf, sizeof(buf), "UP:%04X STOP:%04X DN:%04X",
            r->cmd_up, r->cmd_stop, r->cmd_down);
        canvas_draw_str_aligned(canvas, 64, 48, AlignCenter, AlignTop, buf);
    }

    canvas_draw_str(canvas, 0, 63, "Back:Cancel");
    if(app->learn_btn >= 3) {
        canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, "OK:Save");
    }
}

static void dooya_draw_cb(Canvas* canvas, void* ctx) {
    DooyaApp* app = ctx;
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    if(app->mode == DooyaModeLearn) {
        dooya_draw_learn(canvas, app);
    } else {
        dooya_draw_remote(canvas, app);
    }
}

// ============== Input ==============
static void dooya_input_cb(InputEvent* ev, void* ctx) {
    furi_message_queue_put(((DooyaApp*)ctx)->event_queue, ev, FuriWaitForever);
}

static void dooya_handle_learn_frame(DooyaApp* app, uint64_t frame) {
    uint32_t id = (frame >> 40) & 0xFFFFFF;
    uint32_t addr = (frame >> 16) & 0xFFFFFF;
    uint16_t cmd = frame & 0xFFFF;

    // Skip confirm frames (same ID+addr, different cmd after main cmd)
    uint8_t slot = app->remote_count < DOOYA_MAX_REMOTES ? app->remote_count : DOOYA_MAX_REMOTES - 1;
    DooyaRemoteData* r = &app->remotes[slot];

    if(app->learn_btn == 0) {
        // First button — store ID and addr
        r->id = id;
        r->addr = addr;
        r->cmd_up = cmd;
        r->cmd_confirm = DOOYA_CMD_CONFIRM; // default, may update
        snprintf(r->name, sizeof(r->name), "Learned %d", slot + 1);
        app->learn_btn = 1;
        notification_message(app->notifications, &sequence_success);
    } else if(id == r->id && addr == r->addr) {
        // Same remote — assign to next button
        if(app->learn_btn == 1 && cmd != r->cmd_up) {
            r->cmd_stop = cmd;
            app->learn_btn = 2;
            notification_message(app->notifications, &sequence_success);
        } else if(app->learn_btn == 2 && cmd != r->cmd_up && cmd != r->cmd_stop) {
            r->cmd_down = cmd;
            // Check if next frame is confirm
            r->cmd_confirm = DOOYA_CMD_CONFIRM;
            app->learn_btn = 3;
            notification_message(app->notifications, &sequence_success);
        }
    }
    // Ignore frames from different remotes or duplicate commands
    view_port_update(app->view_port);
}

// ============== Main ==============
int32_t dooya_remote_app(void* p) {
    UNUSED(p);
    DooyaApp* app = malloc(sizeof(DooyaApp));
    memset(app, 0, sizeof(DooyaApp));
    app->running = true;
    app->mode = DooyaModeRemote;
    app->upload = malloc(DOOYA_UPLOAD_MAX * sizeof(LevelDuration));

    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    subghz_devices_init();
    app->radio = radio_device_loader_set(NULL, SubGhzRadioDeviceTypeInternal);
    app->worker = subghz_worker_alloc();

    dooya_load(app);

    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, dooya_draw_cb, app);
    view_port_input_callback_set(app->view_port, dooya_input_cb, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    InputEvent event;
    while(app->running) {
        // In learn mode, check for decoded frames
        if(app->mode == DooyaModeLearn && app->rx_frame_ready) {
            app->rx_frame_ready = false;
            dooya_handle_learn_frame(app, app->rx_frame);
        }

        if(furi_message_queue_get(app->event_queue, &event, 50) != FuriStatusOk) continue;

        if(app->mode == DooyaModeLearn) {
            // Learn mode input
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                dooya_rx_stop(app);
                app->mode = DooyaModeRemote;
                view_port_update(app->view_port);
            } else if(event.key == InputKeyOk && event.type == InputTypeShort && app->learn_btn >= 3) {
                // Save the learned remote
                dooya_rx_stop(app);
                uint8_t slot = app->remote_count < DOOYA_MAX_REMOTES ? app->remote_count : DOOYA_MAX_REMOTES - 1;
                if(app->remote_count < DOOYA_MAX_REMOTES) app->remote_count++;
                app->remote_sel = slot;
                dooya_save(app);
                app->mode = DooyaModeRemote;
                notification_message(app->notifications, &sequence_success);
                view_port_update(app->view_port);
            }
        } else {
            // Remote mode input
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                app->running = false;
            } else if(event.key == InputKeyOk && event.type == InputTypeLong) {
                // Menu — no transmit on long press
                if(app->remote_count < DOOYA_MAX_REMOTES) {
                    app->mode = DooyaModeLearn;
                    app->learn_btn = 0;
                    dooya_rx_start(app);
                    view_port_update(app->view_port);
                }
            } else if(event.key == InputKeyOk && event.type == InputTypeShort) {
                // STOP — fires on release, so long-press OK won't trigger this
                DooyaRemoteData* r = &app->remotes[app->remote_sel];
                app->last_cmd = 2;
                dooya_transmit(app, r->cmd_stop, false);
                notification_message(app->notifications, &sequence_blink_cyan_100);
            } else if(event.key == InputKeyLeft && event.type == InputTypeShort) {
                if(app->remote_sel > 0) app->remote_sel--;
                else app->remote_sel = app->remote_count - 1;
                view_port_update(app->view_port);
            } else if(event.key == InputKeyRight && event.type == InputTypeShort) {
                app->remote_sel = (app->remote_sel + 1) % app->remote_count;
                view_port_update(app->view_port);
            } else if(event.type == InputTypePress) {
                DooyaRemoteData* r = &app->remotes[app->remote_sel];
                uint16_t cmd = 0;
                bool confirm = false;
                uint8_t cmd_id = 0;
                switch(event.key) {
                case InputKeyUp:   cmd = r->cmd_up;   confirm = true;  cmd_id = 1; break;
                case InputKeyDown: cmd = r->cmd_down;  confirm = true;  cmd_id = 3; break;
                default: break;
                }
                if(cmd) {
                    if(app->transmitting) {
                        app->pending_cmd = cmd;
                        app->pending_confirm = confirm;
                        app->last_cmd = cmd_id;
                    } else {
                        app->last_cmd = cmd_id;
                        dooya_transmit(app, cmd, confirm);
                        notification_message(app->notifications, &sequence_blink_cyan_100);
                        if(app->pending_cmd) {
                            uint16_t pc = app->pending_cmd;
                            bool pcf = app->pending_confirm;
                            app->pending_cmd = 0;
                            dooya_transmit(app, pc, pcf);
                            notification_message(app->notifications, &sequence_blink_cyan_100);
                        }
                    }
                }
            }
        }
    }

    subghz_devices_idle(app->radio);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);
    subghz_worker_free(app->worker);
    radio_device_loader_end(app->radio);
    subghz_devices_deinit();
    furi_record_close(RECORD_NOTIFICATION);
    furi_message_queue_free(app->event_queue);
    free(app->upload);
    free(app);
    return 0;
}
