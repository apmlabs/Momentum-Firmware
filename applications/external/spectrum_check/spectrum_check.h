#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>
#include <lib/flipper_format/flipper_format.h>
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/subghz_setting.h>
#include <lib/subghz/receiver.h>
#include <lib/subghz/registry.h>
#include <lib/subghz/protocols/base.h>
#include <lib/subghz/environment.h>
#include <lib/drivers/cc1101_regs.h>
#include "helpers/radio_device_loader.h"

#define TAG "SpectrumCheck"

// Spectrum
#define SC_NUM_CHANNELS 102
#define SC_FREQ_BOTTOM_Y 50
#define SC_FREQ_START_X 14

// Signal library
#define SC_MAX_SIGNALS 8
#define SC_RAW_PER_SIGNAL 512

// Thresholds
#define SC_RSSI_MIN (-97.0f)
#define SC_RSSI_MAX (-60.0f)
#define SC_TRIGGER_STEP 1

// Views
typedef enum {
    SCViewSpectrum,
    SCViewFreqAnalyzer,
    SCViewDecoder,
    SCViewWaveform,
    SCViewCount,
} SCView;

typedef enum {
    SCWidthWide,
    SCWidthNarrow,
    SCWidthUltraWide,
} SCWidth;

typedef enum {
    SCLogSortCount,
    SCLogSortRSSI,
    SCLogSortFreq,
    SCLogSortRecent,
    SCLogSortModes,
} SCLogSort;

// Modulation presets
typedef enum {
    SCModAM650,
    SCModAM270,
    SCModFM238,
    SCModFM476,
    SCModCount,
} SCMod;

// Frequency hit log entry
typedef struct {
    uint32_t frequency;
    uint8_t count;
    uint8_t rssi_max;
    uint8_t seq;
} SCLogEntry;

// One captured signal with its raw data
typedef struct {
    uint32_t frequency;
    SCMod modulation;
    int32_t raw_data[SC_RAW_PER_SIGNAL]; // +duration=high, -duration=low
    uint16_t raw_count;
    // Decoded info
    uint16_t pulse_count;
    uint32_t total_duration_us;
    uint32_t min_pulse_us;
    uint32_t est_rate_hz;
    bool analyzed;
    // Protocol decode result
    char protocol_name[32];
    char decoded_string[128];
    bool protocol_decoded;
} SCSignal;

// Main app state
typedef struct {
    // System
    ViewPort* view_port;
    Gui* gui;
    FuriMessageQueue* event_queue;
    FuriMutex* mutex;
    NotificationApp* notifications;
    bool running;

    // Radio
    const SubGhzDevice* radio_device;
    SubGhzEnvironment* environment;
    SubGhzReceiver* receiver;
    // Layer 3: extra protocols (weather, tpms, pocsag)
    SubGhzEnvironment* extra_environment;
    SubGhzReceiver* extra_receiver;

    // Current state
    SCView current_view;
    uint32_t frequency;
    SCMod modulation;
    SCWidth width;

    // Spectrum data
    uint8_t channel_ss[SC_NUM_CHANNELS];
    float max_rssi;
    uint8_t max_rssi_channel;

    // Frequency analyzer
    float trigger;
    bool signal_found;
    uint32_t detected_freq;
    float detected_rssi;
    #define SC_MAX_LOG 16
    SCLogEntry log[SC_MAX_LOG];
    uint8_t log_size;
    uint8_t log_seq;
    SCLogSort log_sort;
    uint8_t log_scroll;

    // Signal library
    SCSignal signals[SC_MAX_SIGNALS];
    uint8_t signal_count;
    uint8_t signal_selected; // which signal we're viewing in decoder/waveform

    // Waveform
    uint16_t waveform_scroll;
    uint8_t waveform_zoom;

    // Capture state
    bool capturing;       // currently capturing raw data
    bool capture_done;    // capture finished, data held

    // Worker
    FuriThread* worker_thread;
    bool worker_running;
} SpectrumCheckApp;

int32_t spectrum_check_app(void* p);
