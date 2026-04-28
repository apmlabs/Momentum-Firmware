#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/modules/text_input.h>
#include <gui/view_dispatcher.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>
#include <lib/flipper_format/flipper_format.h>
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/subghz_setting.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/subghz/receiver.h>
#include <lib/subghz/registry.h>
#include <lib/subghz/protocols/base.h>
#include <lib/subghz/protocols/bin_raw.h>
#include <lib/subghz/environment.h>
#include <lib/drivers/cc1101_regs.h>
#include "helpers/radio_device_loader.h"

#define TAG "SpectrumCheck"

#define SC_HIT_LOG_SIZE 16
#define SC_SIGNAL_SLOTS 8
#define SC_RAW_SAMPLES  1024
#define SC_SIG_SAMPLES  512
#define SC_SPEC_CH      32
#define SC_RSSI_MIN     (-97.0f)
#define SC_RSSI_MAX     (-60.0f)
#define SC_TRIGGER_STEP 1

typedef enum { SCViewSpectrum, SCViewFreqAnalyzer, SCViewCamp, SCViewDecoder, SCViewSettings, SCViewCount } SCView;
typedef enum { SCRadioHopping, SCRadioLocked, SCRadioPaused } SCRadioState;
typedef enum {
    SCModAM650, SCModAM270, SCModFM238, SCModFM476,
    SCModTPMS_FSK, SCModTPMS_OOK, SCModTPMS_GFSK, SCModOOK_40k, SCModFSK_40k, SCModCount
} SCMod;
typedef enum { SCSortCount, SCSortRSSI, SCSortFreq, SCSortRecent, SCSortModes } SCSort;

typedef struct {
    uint32_t frequency;
    uint8_t  count;
    int8_t   rssi_max;
    uint8_t  seq;
    char     protocol[20];
} SCHit;

typedef struct {
    uint32_t frequency;
    SCMod    modulation;
    int32_t  raw_data[SC_SIG_SAMPLES];
    uint16_t raw_count;
    uint16_t pulse_count;
    uint32_t total_duration_us;
    uint32_t min_pulse_us;
    bool     analyzed;
    char     protocol_name[32];
    char     decoded_string[128];
    bool     protocol_decoded;
} SCSignal;

typedef struct {
    // System
    ViewPort* view_port;
    Gui* gui;
    FuriMessageQueue* event_queue;
    NotificationApp* notifications;
    bool running;

    // Radio
    const SubGhzDevice* radio_device;
    SubGhzWorker* worker;
    SubGhzEnvironment* environment;
    SubGhzReceiver* receiver;
    SubGhzEnvironment* extra_environment;
    SubGhzReceiver* extra_receiver;
    SubGhzProtocolDecoderBinRAW* bin_raw_decoder;
    bool rx_active;

    // Radio state
    SCRadioState radio_state;
    uint32_t current_freq;
    SCMod    current_mod;
    float    trigger;
    float    noise_floor;     // Adaptive noise floor estimate (dBm)
    uint8_t  hopper_idx;
    uint8_t  hopper_timeout;
    uint32_t locked_freq;
    uint8_t  locked_mod_idx;
    bool     was_on_signal; // prevents duplicate hit logging per hop

    // Modulation rotation (separate from hopper_idx)
    uint8_t  mod_rotation;

    // Spectrum
    uint8_t  spec_peak[SC_SPEC_CH];
    float    spec_held_rssi;
    uint8_t  spec_held_ch;
    uint32_t spec_held_tick;
    uint8_t  spec_decay;
    uint8_t  spec_bw; // 0=Wide(650kHz), 1=Medium(270kHz), 2=Narrow(58kHz)
    uint8_t  spec_band; // 0=All, 1=300MHz, 2=433MHz, 3=800MHz

    // Hit Log
    SCHit    hits[SC_HIT_LOG_SIZE];
    uint8_t  hit_count;
    uint8_t  hit_seq;
    SCSort   hit_sort;
    uint8_t  hit_cursor;

    // Signal Library
    SCSignal signals[SC_SIGNAL_SLOTS];
    uint8_t  signal_count;
    uint8_t  signal_selected;

    // Raw circular buffer (filled by worker pair callback)
    int32_t  raw_buf[SC_RAW_SAMPLES];
    volatile uint16_t raw_write;

    // Pending decode (lock-free: worker writes, main loop reads)
    volatile bool pending_decode;
    char     pending_name[32];
    char     pending_str[128];
    uint32_t pending_freq;
    uint8_t  pending_mod;

    // UI
    SCView   current_view;

    // Save (keyboard)
    ViewDispatcher* view_dispatcher;
    bool     show_keyboard;
    char     save_filename[32];
    uint8_t  save_slot;
    uint8_t  decoder_scroll;

    // Camp mode
    uint8_t  camp_mod_idx; // index into sc_try_mods[]
    uint32_t camp_start_tick;
    char     camp_last_proto[32];

    // Settings
    uint8_t  settings_digit; // 0-8: which digit of freq to edit (xxx.xxx.xxx)
} SpectrumCheckApp;

int32_t spectrum_check_app(void* p);
