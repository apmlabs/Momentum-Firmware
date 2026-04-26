/*
 * Dooya Remote — Motorized blind controller
 * Buttons are learned individually, like the IR app.
 * Each button = a named 16-bit command on a 64-bit Dooya frame.
 */
#include "dooya_remote.h"
#define TAG "DooyaRemote"

// ============== File I/O ==============
static void dooya_save(DooyaApp* app) {
    Storage* st = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(st, DOOYA_DIR);
    FlipperFormat* ff = flipper_format_file_alloc(st);
    if(flipper_format_file_open_always(ff, DOOYA_SAVE_FILE)) {
        flipper_format_write_header_cstr(ff, "Dooya Remotes", 1);
        uint32_t cnt = app->remote_count;
        flipper_format_write_uint32(ff, "Count", &cnt, 1);
        for(uint8_t r = 0; r < app->remote_count; r++) {
            DooyaRemoteData* rem = &app->remotes[r];
            uint32_t hdr[3] = {rem->id, rem->addr, rem->btn_count};
            flipper_format_write_string_cstr(ff, "Remote", rem->name);
            flipper_format_write_uint32(ff, "Hdr", hdr, 3);
            for(uint8_t b = 0; b < rem->btn_count; b++) {
                flipper_format_write_string_cstr(ff, "Btn", rem->buttons[b].name);
                uint32_t f[2] = {(uint32_t)(rem->buttons[b].frame >> 32), (uint32_t)rem->buttons[b].frame};
                flipper_format_write_uint32(ff, "Frm", f, 2);
            }
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
            // Detect format: try reading first Frm key to see if it exists
            bool has_frm = false;
            {
                uint32_t probe[2] = {0};
                // Save position by trying to read — if Frm exists anywhere, use new format
                // We need to rewind after probing
                if(flipper_format_read_uint32(ff, "Frm", probe, 2)) has_frm = true;
                flipper_format_rewind(ff);
                // Re-skip header and Count
                flipper_format_read_header(ff, type, &ver);
                flipper_format_read_uint32(ff, "Count", &cnt, 1);
                if(cnt > DOOYA_MAX_REMOTES) cnt = DOOYA_MAX_REMOTES;
            }
            FuriString* s = furi_string_alloc();
            for(uint32_t r = 0; r < cnt; r++) {
                DooyaRemoteData* rem = &app->remotes[app->remote_count];
                memset(rem, 0, sizeof(DooyaRemoteData));
                if(!flipper_format_read_string(ff, "Remote", s)) break;
                snprintf(rem->name, DOOYA_NAME_LEN, "%s", furi_string_get_cstr(s));
                uint32_t hdr[3] = {0};
                if(!flipper_format_read_uint32(ff, "Hdr", hdr, 3)) break;
                rem->id = hdr[0]; rem->addr = hdr[1];
                uint8_t bc = hdr[2] > DOOYA_MAX_BTNS ? DOOYA_MAX_BTNS : hdr[2];
                for(uint8_t b = 0; b < bc; b++) {
                    if(!flipper_format_read_string(ff, "Btn", s)) break;
                    snprintf(rem->buttons[b].name, DOOYA_NAME_LEN, "%s", furi_string_get_cstr(s));
                    uint32_t f[2] = {0};
                    bool got = false;
                    if(has_frm) {
                        got = flipper_format_read_uint32(ff, "Frm", f, 2);
                        if(got) rem->buttons[b].frame = ((uint64_t)f[0] << 32) | f[1];
                    } else {
                        got = flipper_format_read_uint32(ff, "Cmd", f, 1);
                        if(got) rem->buttons[b].frame = ((uint64_t)rem->id << 40) | ((uint64_t)rem->addr << 16) | (uint16_t)f[0];
                    }
                    if(!got) break;
                    rem->btn_count++;
                }
                app->remote_count++;
            }
            furi_string_free(s);
        }
        furi_string_free(type);
    }
    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);

    // Default remote if nothing saved
    if(app->remote_count == 0) {
        DooyaRemoteData* rem = &app->remotes[0];
        rem->id = 0xA3C0A1; rem->addr = 0x6C0100;
        snprintf(rem->name, DOOYA_NAME_LEN, "Default");
        rem->buttons[0] = (DooyaButton){.frame = 0xA3C0A16C01000BD9}; snprintf(rem->buttons[0].name, DOOYA_NAME_LEN, "Up");
        rem->buttons[1] = (DooyaButton){.frame = 0xA3C0A16C010023F1}; snprintf(rem->buttons[1].name, DOOYA_NAME_LEN, "Stop");
        rem->buttons[2] = (DooyaButton){.frame = 0xA3C0A16C01004311}; snprintf(rem->buttons[2].name, DOOYA_NAME_LEN, "Down");
        rem->buttons[3] = (DooyaButton){.frame = 0xA3C0A16C010024F2}; snprintf(rem->buttons[3].name, DOOYA_NAME_LEN, "Confirm");
        rem->btn_count = 4;
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

static void dooya_transmit(DooyaApp* app, uint64_t frame) {
    app->transmitting = true;
    view_port_update(app->view_port);
    uint16_t pos = 0;
    for(uint8_t i = 0; i < DOOYA_REPEATS; i++)
        pos = dooya_encode_frame(app->upload, pos, frame);
    app->upload_size = pos; app->upload_idx = 0;
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
static void dooya_rx_cb(void* ctx, bool level, uint32_t duration) {
    DooyaApp* app = ctx;
    switch(app->rx_state) {
    case RxIdle:
        if(level && duration > 180 && duration < 450) { app->rx_pre_count = 1; app->rx_state = RxPreamble; }
        break;
    case RxPreamble:
        if(!level && duration > 400 && duration < 800) { /* preamble LOW */ }
        else if(level && duration > 180 && duration < 450) { app->rx_pre_count++; }
        else if(level && duration > 3500 && duration < 6500) { app->rx_state = app->rx_pre_count >= 4 ? RxSync : RxIdle; }
        else { app->rx_state = RxIdle; }
        break;
    case RxSync:
        if(!level && duration > 300 && duration < 1000) { app->rx_bit_count = 0; app->rx_data = 0; app->rx_state = RxData; }
        else { app->rx_state = RxIdle; }
        break;
    case RxData:
        if(level) {
            app->rx_data <<= 1;
            if(duration > 400) app->rx_data |= 1;
            if(++app->rx_bit_count >= 64) { app->rx_frame = app->rx_data; app->rx_frame_ready = true; app->rx_state = RxIdle; }
        } else if(duration > 3500) {
            if(app->rx_bit_count >= 48) { app->rx_frame = app->rx_data << (64 - app->rx_bit_count); app->rx_frame_ready = true; }
            app->rx_state = RxIdle;
        }
        break;
    }
}

static void dooya_rx_start(DooyaApp* app) {
    app->rx_state = RxIdle; app->rx_frame_ready = false;
    subghz_devices_idle(app->radio);
    subghz_devices_load_preset(app->radio, FuriHalSubGhzPresetOok650Async, NULL);
    subghz_devices_set_frequency(app->radio, 433920000);
    subghz_worker_set_pair_callback(app->worker, (SubGhzWorkerPairCallback)dooya_rx_cb);
    subghz_worker_set_context(app->worker, app);
    subghz_devices_start_async_rx(app->radio, subghz_worker_rx_callback, app->worker);
    subghz_worker_start(app->worker);
}

static void dooya_rx_stop(DooyaApp* app) {
    subghz_worker_stop(app->worker);
    subghz_devices_stop_async_rx(app->radio);
    subghz_devices_idle(app->radio);
}

// ============== Blocking UI helpers ==============
static void dooya_menu_cb(void* ctx, uint32_t index) {
    DooyaApp* app = ctx; app->menu_result = index; view_dispatcher_stop(app->vd);
}
static bool dooya_nav_cb(void* ctx) {
    DooyaApp* app = ctx; app->menu_result = UINT32_MAX; view_dispatcher_stop(app->vd); return true;
}
static void dooya_text_cb(void* ctx) {
    DooyaApp* app = ctx; app->menu_result = 0; view_dispatcher_stop(app->vd);
}

static uint32_t dooya_show_menu(DooyaApp* app, const char* header, const char** items, uint8_t count) {
    gui_remove_view_port(app->gui, app->view_port);
    ViewDispatcher* vd = view_dispatcher_alloc();
    Submenu* sm = submenu_alloc();
    app->vd = vd; app->menu_result = UINT32_MAX;
    view_dispatcher_set_event_callback_context(vd, app);
    view_dispatcher_set_navigation_event_callback(vd, dooya_nav_cb);
    view_dispatcher_add_view(vd, 0, submenu_get_view(sm));
    if(header) submenu_set_header(sm, header);
    for(uint8_t i = 0; i < count; i++) submenu_add_item(sm, items[i], i, dooya_menu_cb, app);
    view_dispatcher_switch_to_view(vd, 0);
    view_dispatcher_attach_to_gui(vd, app->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_run(vd);
    view_dispatcher_remove_view(vd, 0);
    submenu_free(sm); view_dispatcher_free(vd); app->vd = NULL;
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    return app->menu_result;
}

static bool dooya_show_keyboard(DooyaApp* app, const char* header, char* buf, uint8_t len) {
    gui_remove_view_port(app->gui, app->view_port);
    ViewDispatcher* vd = view_dispatcher_alloc();
    TextInput* ti = text_input_alloc();
    app->vd = vd; app->menu_result = UINT32_MAX;
    view_dispatcher_set_event_callback_context(vd, app);
    view_dispatcher_set_navigation_event_callback(vd, dooya_nav_cb);
    view_dispatcher_add_view(vd, 0, text_input_get_view(ti));
    text_input_set_header_text(ti, header);
    text_input_set_result_callback(ti, dooya_text_cb, app, buf, len, false);
    view_dispatcher_switch_to_view(vd, 0);
    view_dispatcher_attach_to_gui(vd, app->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_run(vd);
    bool ok = app->menu_result != UINT32_MAX;
    view_dispatcher_remove_view(vd, 0);
    text_input_free(ti); view_dispatcher_free(vd); app->vd = NULL;
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    return ok;
}

// ============== Scan: brute-force TX ==============
static const char* DOOYA_BTN_NAMES[] = {"UP", "DOWN", "STOP"};

static uint16_t dooya_calc_channel_field(uint8_t ch) {
    if(ch == 0) return 0x0000;
    return (uint16_t)(1 << ((ch + 7) % 16));
}

static void dooya_scan_transmit(DooyaApp* app) {
    app->transmitting = true;
    view_port_update(app->view_port);

    static const uint8_t CMD_BYTES[] = {0x0B, 0x43, 0x23}; // UP, DOWN, STOP
    static const uint8_t CMD_CONFIRM = 0x24;

    uint16_t ch_field = dooya_calc_channel_field(app->scan_ch);
    uint8_t id0 = 0xC0, id1 = (app->scan_id16 >> 8) & 0xFF, id2 = app->scan_id16 & 0xFF;
    uint8_t addr_hi = (ch_field >> 8) & 0xFF, addr_lo = ch_field & 0xFF;
    uint8_t cmd = CMD_BYTES[app->scan_btn];
    uint8_t chk = (id0 + id1 + id2 + addr_hi + addr_lo + cmd) & 0xFF;
    uint64_t data = ((uint64_t)0xA3 << 56) | ((uint64_t)id0 << 48) | ((uint64_t)id1 << 40) |
                    ((uint64_t)id2 << 32) | ((uint64_t)ch_field << 16) |
                    ((uint64_t)cmd << 8) | chk;

    uint16_t pos = 0;
    for(uint8_t i = 0; i < DOOYA_REPEATS; i++)
        pos = dooya_encode_frame(app->upload, pos, data);

    // UP and DOWN get confirm burst; STOP doesn't
    if(app->scan_btn != 2) {
        uint8_t chk_c = (id0 + id1 + id2 + addr_hi + addr_lo + CMD_CONFIRM) & 0xFF;
        uint64_t data_c = ((uint64_t)0xA3 << 56) | ((uint64_t)id0 << 48) | ((uint64_t)id1 << 40) |
                          ((uint64_t)id2 << 32) | ((uint64_t)ch_field << 16) |
                          ((uint64_t)CMD_CONFIRM << 8) | chk_c;
        for(uint8_t i = 0; i < DOOYA_REPEATS; i++)
            pos = dooya_encode_frame(app->upload, pos, data_c);
    }

    app->upload_size = pos; app->upload_idx = 0;
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

static bool dooya_scan_advance(DooyaApp* app) {
    if(app->scan_id16 < 0xFFFF) {
        app->scan_id16++;
    } else {
        return false; // done all 65536
    }
    return true;
}

// ============== Drawing ==============
static void dooya_draw_scan(Canvas* canvas, DooyaApp* app) {
    static const uint8_t CMD_BYTES[] = {0x0B, 0x43, 0x23};
    char buf[40];
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 0, AlignCenter, AlignTop, "Scan Remotes");

    canvas_set_font(canvas, FontSecondary);
    snprintf(buf, sizeof(buf), "ID: C0%04X  [%s]", app->scan_id16, DOOYA_BTN_NAMES[app->scan_btn]);
    canvas_draw_str_aligned(canvas, 64, 14, AlignCenter, AlignTop, buf);

    snprintf(buf, sizeof(buf), "ID:C0%04X Cmd:0x%02X", app->scan_id16, CMD_BYTES[app->scan_btn]);
    canvas_draw_str_aligned(canvas, 64, 26, AlignCenter, AlignTop, buf);

    snprintf(buf, sizeof(buf), "%u / 65536  (%lu%%)", app->scan_id16, (uint32_t)app->scan_id16 * 100 / 65536);
    canvas_draw_str_aligned(canvas, 64, 38, AlignCenter, AlignTop, buf);

    if(app->transmitting) {
        canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignTop, ">>> TX <<<");
    } else if(app->scan_running) {
        canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignTop, "<>:Btn  OK:Pause");
    } else {
        canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignTop, "OK:Send ^v:Step <>:Btn");
    }

    canvas_draw_str(canvas, 0, 63, "Back:Exit");
    canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom,
        app->scan_running ? "Running" : "Hold OK:Auto");
}
static void dooya_draw_remote(Canvas* canvas, DooyaApp* app) {
    DooyaRemoteData* rem = &app->remotes[app->remote_sel];
    char buf[32];

    // Title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 0, AlignCenter, AlignTop, rem->name);

    // Status
    canvas_set_font(canvas, FontSecondary);
    if(app->transmitting) {
        canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignTop, ">>> Transmitting <<<");
    } else {
        snprintf(buf, sizeof(buf), "%06lX:%06lX  %d/%d",
            rem->id, rem->addr, app->remote_sel + 1, app->remote_count);
        canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignTop, buf);
    }

    // Button list — show up to 3 visible, selected one highlighted
    if(rem->btn_count == 0) {
        canvas_draw_str_aligned(canvas, 64, 34, AlignCenter, AlignCenter, "No buttons");
    } else {
        uint8_t first = 0;
        if(app->btn_sel > 1) first = app->btn_sel - 1;
        if(first + 3 > rem->btn_count && rem->btn_count >= 3) first = rem->btn_count - 3;
        for(uint8_t i = 0; i < 3 && (first + i) < rem->btn_count; i++) {
            uint8_t idx = first + i;
            uint8_t y = 22 + i * 12;
            canvas_draw_rframe(canvas, 20, y, 88, 11, 3);
            if(idx == app->btn_sel) {
                canvas_draw_rbox(canvas, 20, y, 88, 11, 3);
                canvas_set_color(canvas, ColorWhite);
            }
            snprintf(buf, sizeof(buf), "%s [%02X]", rem->buttons[idx].name, (uint8_t)(rem->buttons[idx].frame >> 8));
            canvas_draw_str_aligned(canvas, 64, y + 2, AlignCenter, AlignTop, buf);
            canvas_set_color(canvas, ColorBlack);
        }
        // Scroll indicators
        if(first > 0) canvas_draw_str_aligned(canvas, 6, 28, AlignCenter, AlignCenter, "\x18");
        if(first + 3 < rem->btn_count) canvas_draw_str_aligned(canvas, 6, 40, AlignCenter, AlignCenter, "\x19");
    }

    // L/R remote switch
    canvas_draw_str_aligned(canvas, 2, 34, AlignLeft, AlignCenter, "<");
    canvas_draw_str_aligned(canvas, 126, 34, AlignRight, AlignCenter, ">");

    // Bottom
    canvas_draw_str(canvas, 0, 63, "OK:Send");
    canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, "Hold OK:Menu");
}

static void dooya_draw_learn(Canvas* canvas, DooyaApp* app) {
    DooyaRemoteData* rem = &app->remotes[app->remote_sel];
    char buf[40];
    uint8_t new_count = rem->btn_count - app->learn_start;

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 0, AlignCenter, AlignTop, "Learn Buttons");
    canvas_set_font(canvas, FontSecondary);

    if(new_count == 0) {
        canvas_draw_str_aligned(canvas, 64, 20, AlignCenter, AlignTop, "Press any button on remote");
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, "Listening 433.92 MHz...");
    } else {
        snprintf(buf, sizeof(buf), "%06lX:%06lX  New:%d", rem->id, rem->addr, new_count);
        canvas_draw_str_aligned(canvas, 64, 13, AlignCenter, AlignTop, buf);
        uint8_t first = app->learn_start;
        if(rem->btn_count > first + 3) first = rem->btn_count - 3;
        for(uint8_t i = 0; i < 3 && (first + i) < rem->btn_count; i++) {
            DooyaButton* b = &rem->buttons[first + i];
            snprintf(buf, sizeof(buf), "+ %s  [%02X]", b->name, (uint8_t)(b->frame >> 8));
            canvas_draw_str(canvas, 4, 28 + i * 10, buf);
        }
    }

    canvas_draw_str(canvas, 0, 63, "Back:Done");
    canvas_draw_str_aligned(canvas, 127, 63, AlignRight, AlignBottom, "Listening...");
}

static void dooya_draw_cb(Canvas* canvas, void* ctx) {
    DooyaApp* app = ctx;
    canvas_clear(canvas); canvas_set_color(canvas, ColorBlack);
    if(app->mode == DooyaModeLearn) dooya_draw_learn(canvas, app);
    else if(app->mode == DooyaModeScan) dooya_draw_scan(canvas, app);
    else dooya_draw_remote(canvas, app);
}

static void dooya_input_cb(InputEvent* ev, void* ctx) {
    furi_message_queue_put(((DooyaApp*)ctx)->event_queue, ev, FuriWaitForever);
}

// ============== Learn: auto-save each unique signal ==============
static void dooya_handle_learn_frame(DooyaApp* app, uint64_t frame) {
    uint32_t id = (frame >> 40) & 0xFFFFFF;
    uint32_t addr = (frame >> 16) & 0xFFFFFF;

    DooyaRemoteData* rem = &app->remotes[app->remote_sel];

    // First frame sets ID+addr
    if(rem->btn_count == 0) {
        rem->id = id; rem->addr = addr;
    }

    // Check if this cmd already captured (skip — save all for testing)
    // for(uint8_t i = 0; i < rem->btn_count; i++) {
    //     if(rem->buttons[i].cmd == cmd) return; // duplicate
    // }

    // Add new button
    if(rem->btn_count < DOOYA_MAX_BTNS) {
        DooyaButton* btn = &rem->buttons[rem->btn_count];
        snprintf(btn->name, DOOYA_NAME_LEN, "Button %d", rem->btn_count + 1);
        btn->frame = frame;
        rem->btn_count++;
        dooya_save(app); // save immediately
        notification_message(app->notifications, &sequence_success);
    }
    view_port_update(app->view_port);
}

// ============== Main ==============
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
    app->worker = subghz_worker_alloc();
    dooya_load(app);

    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, dooya_draw_cb, app);
    view_port_input_callback_set(app->view_port, dooya_input_cb, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    InputEvent event;
    while(app->running) {
        if(app->mode == DooyaModeLearn && app->rx_frame_ready) {
            app->rx_frame_ready = false;
            dooya_handle_learn_frame(app, app->rx_frame);
        }

        // Auto-scan: transmit and advance when no input pending
        if(app->mode == DooyaModeScan && app->scan_running) {
            if(furi_message_queue_get(app->event_queue, &event, 0) == FuriStatusOk) {
                // Got input during scan — handle it below
            } else {
                // Skip known remotes
                bool skip = false;
                for(uint8_t r = 0; r < app->remote_count && !skip; r++) {
                    // Compare bytes 1+2 of remote ID from stored frames
                    if(app->remotes[r].btn_count > 0) {
                        uint64_t f = app->remotes[r].buttons[0].frame;
                        uint16_t stored_id16 = (uint16_t)(((f >> 40) & 0xFF) << 8 | ((f >> 32) & 0xFF));
                        if(stored_id16 == app->scan_id16) skip = true;
                    }
                }
                if(!skip) dooya_scan_transmit(app);
                else furi_delay_ms(10);
                furi_delay_ms(200); // pause between codes
                if(!dooya_scan_advance(app)) {
                    app->scan_running = false; // finished all combos
                }
                view_port_update(app->view_port);
                continue;
            }
        } else {
            if(furi_message_queue_get(app->event_queue, &event, 50) != FuriStatusOk) continue;
        }

        if(app->mode == DooyaModeLearn) {
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                dooya_rx_stop(app);
                app->mode = DooyaModeRemote;
                view_port_update(app->view_port);
            }
            continue;
        }

        if(app->mode == DooyaModeScan) {
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                app->scan_running = false;
                app->mode = DooyaModeRemote;
            } else if(event.key == InputKeyOk && event.type == InputTypeShort) {
                if(app->scan_running) {
                    app->scan_running = false; // pause
                } else {
                    dooya_scan_transmit(app); // single shot
                }
            } else if(event.key == InputKeyOk && event.type == InputTypeLong) {
                app->scan_running = !app->scan_running; // toggle auto
            } else if(event.key == InputKeyLeft && event.type == InputTypeShort) {
                app->scan_btn = app->scan_btn == 0 ? 2 : app->scan_btn - 1;
            } else if(event.key == InputKeyRight && event.type == InputTypeShort) {
                app->scan_btn = app->scan_btn >= 2 ? 0 : app->scan_btn + 1;
            } else if(!app->scan_running) {
                if(event.key == InputKeyUp && event.type == InputTypeShort) {
                    app->scan_id16 += 256; // step byte 1
                } else if(event.key == InputKeyDown && event.type == InputTypeShort) {
                    app->scan_id16 -= 256; // step byte 1
                }
            }
            view_port_update(app->view_port);
            continue;
        }

        // Remote mode
        DooyaRemoteData* rem = &app->remotes[app->remote_sel];
        if(event.key == InputKeyBack && event.type == InputTypeShort) {
            app->running = false;
        } else if(event.key == InputKeyUp && event.type == InputTypeShort) {
            if(app->btn_sel > 0) app->btn_sel--;
            view_port_update(app->view_port);
        } else if(event.key == InputKeyDown && event.type == InputTypeShort) {
            if(rem->btn_count > 0 && app->btn_sel < rem->btn_count - 1) app->btn_sel++;
            view_port_update(app->view_port);
        } else if(event.key == InputKeyLeft && event.type == InputTypeShort) {
            app->remote_sel = app->remote_sel > 0 ? app->remote_sel - 1 : app->remote_count - 1;
            app->btn_sel = 0;
            view_port_update(app->view_port);
        } else if(event.key == InputKeyRight && event.type == InputTypeShort) {
            app->remote_sel = (app->remote_sel + 1) % app->remote_count;
            app->btn_sel = 0;
            view_port_update(app->view_port);
        } else if(event.key == InputKeyOk && event.type == InputTypeShort) {
            // Send selected button
            if(rem->btn_count > 0 && app->btn_sel < rem->btn_count) {
                dooya_transmit(app, rem->buttons[app->btn_sel].frame);
                notification_message(app->notifications, &sequence_blink_cyan_100);
            }
        } else if(event.key == InputKeyOk && event.type == InputTypeLong) {
            // Menu
            const char* items[6]; uint8_t n = 0;
            items[n++] = "Learn buttons";
            items[n++] = "Scan remotes";
            if(rem->btn_count > 0) items[n++] = "Rename button";
            if(rem->btn_count > 0) items[n++] = "Delete button";
            items[n++] = "Rename remote";
            if(app->remote_count > 1) items[n++] = "Delete remote";

            uint32_t sel = dooya_show_menu(app, rem->name, items, n);
            if(sel != UINT32_MAX && sel < n) {
                const char* picked = items[sel];
                if(picked == items[0]) { // Learn
                    // If this is a new remote slot, set it up
                    if(rem->btn_count == 0 && app->remote_count < DOOYA_MAX_REMOTES) {
                        snprintf(rem->name, DOOYA_NAME_LEN, "Remote %d", app->remote_count + 1);
                    }
                    app->mode = DooyaModeLearn;
                    app->learn_start = rem->btn_count;
                    dooya_rx_start(app);
                } else if(!strcmp(picked, "Scan remotes")) {
                    app->scan_id16 = 0;
                    app->scan_ch = 1;
                    app->scan_btn = 0;
                    app->scan_running = false;
                    app->mode = DooyaModeScan;
                } else if(!strcmp(picked, "Rename button")) {
                    snprintf(app->name_buf, DOOYA_NAME_LEN, "%s", rem->buttons[app->btn_sel].name);
                    if(dooya_show_keyboard(app, "Rename button", app->name_buf, DOOYA_NAME_LEN)) {
                        snprintf(rem->buttons[app->btn_sel].name, DOOYA_NAME_LEN, "%s", app->name_buf);
                        dooya_save(app);
                    }
                } else if(!strcmp(picked, "Delete button")) {
                    for(uint8_t i = app->btn_sel; i + 1 < rem->btn_count; i++)
                        rem->buttons[i] = rem->buttons[i + 1];
                    rem->btn_count--;
                    if(app->btn_sel >= rem->btn_count && app->btn_sel > 0) app->btn_sel--;
                    dooya_save(app);
                } else if(!strcmp(picked, "Rename remote")) {
                    snprintf(app->name_buf, DOOYA_NAME_LEN, "%s", rem->name);
                    if(dooya_show_keyboard(app, "Rename remote", app->name_buf, DOOYA_NAME_LEN)) {
                        snprintf(rem->name, DOOYA_NAME_LEN, "%s", app->name_buf);
                        dooya_save(app);
                    }
                } else if(!strcmp(picked, "Delete remote")) {
                    for(uint8_t i = app->remote_sel; i + 1 < app->remote_count; i++)
                        app->remotes[i] = app->remotes[i + 1];
                    app->remote_count--;
                    if(app->remote_sel >= app->remote_count) app->remote_sel = app->remote_count - 1;
                    app->btn_sel = 0;
                    dooya_save(app);
                }
            }
            view_port_update(app->view_port);
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
    free(app->upload); free(app);
    return 0;
}
