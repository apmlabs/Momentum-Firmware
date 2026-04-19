#include <math.h>
#include "test_midi.h"
#include "usb/usb_midi_driver.h"
#include "midi/usb_message.h"
#include <furi_hal_speaker.h>

#define MIDI_NOTE_ON_MSG  0x90
#define MIDI_NOTE_OFF_MSG 0x80
#define MIDI_VELOCITY     0x7F

// MIDI note to frequency (Hz) for the ocarina notes
static float midi_note_to_freq(uint8_t note) {
    // f = 440 * 2^((note-69)/12)
    return 440.0f * powf(2.0f, ((float)note - 69.0f) / 12.0f);
}

static void speaker_play(uint8_t note) {
    if(furi_hal_speaker_is_mine() || furi_hal_speaker_acquire(100)) {
        furi_hal_speaker_start(midi_note_to_freq(note), 1.0f);
    }
}

static void speaker_stop(void) {
    if(furi_hal_speaker_is_mine()) {
        furi_hal_speaker_stop();
        furi_hal_speaker_release();
    }
}

void test_midi_draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 15, 10, "MIDI Ocarina");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 28, "Play notes with d-pad!");
    canvas_draw_str(canvas, 2, 40, "OK=C  \x1B=D#  \x18=F  \x1A=G  \x19=A#");
    canvas_draw_str(canvas, 2, 54, "Speaker + USB MIDI output");
    canvas_draw_str(canvas, 2, 64, "Hold Back to exit");
}

void test_midi_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    TestMidiApp* app = ctx;

    uint8_t midi_data[4];
    midi_data[0] = 0x09;
    midi_data[3] = MIDI_VELOCITY;
    uint8_t note = 0;

    if(input_event->type == InputTypePress || input_event->type == InputTypeRelease) {
        if(input_event->key == InputKeyOk) {
            note = 0x48; // C5
        } else if(input_event->key == InputKeyLeft) {
            note = 0x4B; // D#5
        } else if(input_event->key == InputKeyUp) {
            note = 0x4D; // F5
        } else if(input_event->key == InputKeyRight) {
            note = 0x4F; // G5
        } else if(input_event->key == InputKeyDown) {
            note = 0x52; // A#5
        } else if(input_event->key == InputKeyBack) {
            app->should_exit = true;
            return;
        } else {
            return;
        }

        midi_data[2] = note;

        if(input_event->type == InputTypePress) {
            midi_data[1] = MIDI_NOTE_ON_MSG;
            speaker_play(note);
        } else if(input_event->type == InputTypeRelease) {
            midi_data[1] = MIDI_NOTE_OFF_MSG;
            speaker_stop();
        }

        midi_usb_tx(midi_data, 4);
    }
}

TestMidiApp* test_midi_alloc() {
    TestMidiApp* app = malloc(sizeof(TestMidiApp));
    app->midi_note = 0x48;
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, test_midi_draw_callback, app);
    view_port_input_callback_set(app->view_port, test_midi_input_callback, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    app->should_exit = false;
    return app;
}

void test_midi_free(TestMidiApp* app) {
    furi_assert(app);
    speaker_stop();
    gui_remove_view_port(app->gui, app->view_port);
    furi_record_close(RECORD_GUI);
    app->gui = NULL;
    view_port_free(app->view_port);
    free(app);
}

int32_t test_midi_app() {
    FuriHalUsbInterface* usb_mode_prev = furi_hal_usb_get_config();
    furi_hal_usb_set_config(&midi_usb_interface, NULL);
    furi_hal_usb_lock();

    TestMidiApp* app = test_midi_alloc();

    while(!app->should_exit) {
        view_port_update(app->view_port);
        furi_thread_flags_wait(0, FuriFlagWaitAny, 50);
    }

    test_midi_free(app);

    furi_hal_usb_unlock();
    furi_hal_usb_set_config(usb_mode_prev, NULL);

    return 0;
}
