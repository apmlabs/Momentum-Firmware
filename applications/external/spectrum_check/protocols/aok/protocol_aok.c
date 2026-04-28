// A-OK AC114 64-bit protocol decoder for Spectrum Check
// Protocol: 433.92 MHz OOK, 8-pulse preamble + 5ms sync + 64-bit PWM data
// Frame: [0xA3 start][24-bit remote ID][16-bit address][8-bit command][8-bit checksum][trailing 1]
// PWM: short HIGH (~290µs) + long LOW (~600µs) = bit 1, long HIGH (~600µs) + short LOW (~290µs) = bit 0
// Compatible: A-OK, Zemismart, Dooya (64-bit variant), Bofu, Raex

#include "protocol_aok.h"
#include <lib/subghz/protocols/base.h>
#include <lib/subghz/blocks/generic.h>
#include <lib/subghz/blocks/decoder.h>
#include <lib/subghz/blocks/encoder.h>
#include <lib/subghz/blocks/math.h>

#define AOK_TE_SHORT 290
#define AOK_TE_LONG  600
#define AOK_TE_DELTA 100
#define AOK_PREAMBLE_LOW 4800 // ~5ms sync gap after preamble
#define AOK_PREAMBLE_DELTA 1500
#define AOK_MIN_BITS 64

typedef enum {
    AokStepReset,
    AokStepPreamble,   // Counting preamble pulses
    AokStepSync,       // Waiting for sync gap
    AokStepSaveDur,    // Save HIGH duration
    AokStepCheckDur,   // Check LOW duration, decode bit
} AokStep;

typedef struct {
    SubGhzProtocolDecoderBase base;
    SubGhzBlockDecoder decoder;
    SubGhzBlockGeneric generic;
    uint8_t preamble_count;
} SubGhzProtocolDecoderAok;

static void* subghz_protocol_decoder_aok_alloc(SubGhzEnvironment* environment) {
    UNUSED(environment);
    SubGhzProtocolDecoderAok* instance = malloc(sizeof(SubGhzProtocolDecoderAok));
    instance->base.protocol = &subghz_protocol_aok;
    instance->generic.protocol_name = instance->base.protocol->name;
    return instance;
}

static void subghz_protocol_decoder_aok_free(void* context) {
    free(context);
}

static void subghz_protocol_decoder_aok_reset(void* context) {
    SubGhzProtocolDecoderAok* instance = context;
    instance->decoder.parser_step = AokStepReset;
    instance->preamble_count = 0;
}

static void subghz_protocol_decoder_aok_feed(void* context, bool level, uint32_t duration) {
    SubGhzProtocolDecoderAok* instance = context;

    switch(instance->decoder.parser_step) {
    case AokStepReset:
        // Look for short HIGH pulses (preamble)
        if(level && DURATION_DIFF(duration, AOK_TE_SHORT) < AOK_TE_DELTA) {
            instance->preamble_count = 1;
            instance->decoder.parser_step = AokStepPreamble;
        }
        break;

    case AokStepPreamble:
        if(level) {
            // Another short HIGH pulse in preamble
            if(DURATION_DIFF(duration, AOK_TE_SHORT) < AOK_TE_DELTA) {
                instance->preamble_count++;
            } else {
                instance->decoder.parser_step = AokStepReset;
            }
        } else {
            // LOW after preamble pulses
            if(instance->preamble_count >= 4 &&
               DURATION_DIFF(duration, AOK_PREAMBLE_LOW) < AOK_PREAMBLE_DELTA) {
                // Got preamble + sync gap — start decoding data
                instance->decoder.decode_data = 0;
                instance->decoder.decode_count_bit = 0;
                instance->decoder.parser_step = AokStepSaveDur;
            } else if(DURATION_DIFF(duration, AOK_TE_SHORT) < AOK_TE_DELTA) {
                // Short LOW between preamble pulses — continue
            } else {
                instance->decoder.parser_step = AokStepReset;
            }
        }
        break;

    case AokStepSaveDur:
        if(level) {
            instance->decoder.te_last = duration;
            instance->decoder.parser_step = AokStepCheckDur;
        } else {
            instance->decoder.parser_step = AokStepReset;
        }
        break;

    case AokStepCheckDur:
        if(!level) {
            // End of frame (long gap or inter-frame gap)
            if(duration > AOK_TE_LONG * 3) {
                // Add last bit based on saved HIGH duration
                if(DURATION_DIFF(instance->decoder.te_last, AOK_TE_SHORT) < AOK_TE_DELTA) {
                    subghz_protocol_blocks_add_bit(&instance->decoder, 1);
                } else if(DURATION_DIFF(instance->decoder.te_last, AOK_TE_LONG) < AOK_TE_DELTA) {
                    subghz_protocol_blocks_add_bit(&instance->decoder, 0);
                }
                // Check if we got enough bits
                if(instance->decoder.decode_count_bit >= AOK_MIN_BITS) {
                    instance->generic.data = instance->decoder.decode_data;
                    instance->generic.data_count_bit = instance->decoder.decode_count_bit;
                    if(instance->base.callback)
                        instance->base.callback(&instance->base, instance->base.context);
                }
                instance->decoder.parser_step = AokStepReset;
                break;
            }
            // PWM decode: short HIGH + long LOW = 1, long HIGH + short LOW = 0
            if(DURATION_DIFF(instance->decoder.te_last, AOK_TE_SHORT) < AOK_TE_DELTA &&
               DURATION_DIFF(duration, AOK_TE_LONG) < AOK_TE_DELTA) {
                subghz_protocol_blocks_add_bit(&instance->decoder, 1);
                instance->decoder.parser_step = AokStepSaveDur;
            } else if(
                DURATION_DIFF(instance->decoder.te_last, AOK_TE_LONG) < AOK_TE_DELTA &&
                DURATION_DIFF(duration, AOK_TE_SHORT) < AOK_TE_DELTA) {
                subghz_protocol_blocks_add_bit(&instance->decoder, 0);
                instance->decoder.parser_step = AokStepSaveDur;
            } else {
                instance->decoder.parser_step = AokStepReset;
            }
        } else {
            instance->decoder.parser_step = AokStepReset;
        }
        break;
    }
}

static uint8_t subghz_protocol_decoder_aok_get_hash_data(void* context) {
    SubGhzProtocolDecoderAok* instance = context;
    return (uint8_t)(instance->generic.data >> 24);
}

static SubGhzProtocolStatus subghz_protocol_decoder_aok_serialize(
    void* context, FlipperFormat* flipper_format, SubGhzRadioPreset* preset) {
    UNUSED(context); UNUSED(flipper_format); UNUSED(preset);
    return SubGhzProtocolStatusOk;
}

static SubGhzProtocolStatus subghz_protocol_decoder_aok_deserialize(
    void* context, FlipperFormat* flipper_format) {
    UNUSED(context); UNUSED(flipper_format);
    return SubGhzProtocolStatusOk;
}

static void subghz_protocol_decoder_aok_get_string(void* context, FuriString* output) {
    SubGhzProtocolDecoderAok* instance = context;
    uint64_t d = instance->generic.data;
    // Parse A-OK AC114 frame: [start 8][remote_id 24][addr 16][cmd 8][checksum 8]
    uint8_t start = (d >> 56) & 0xFF;
    uint32_t remote_id = (d >> 32) & 0xFFFFFF;
    uint16_t addr = (d >> 16) & 0xFFFF;
    uint8_t cmd = (d >> 8) & 0xFF;
    uint8_t chk = d & 0xFF;

    const char* cmd_name = "?";
    switch(cmd) {
    case 0x0B: cmd_name = "UP"; break;
    case 0x43: cmd_name = "DOWN"; break;
    case 0x23: cmd_name = "STOP"; break;
    case 0x24: cmd_name = "CONFIRM"; break;
    case 0x53: cmd_name = "PROGRAM"; break;
    }

    furi_string_cat_printf(output,
        "%s %dbit\nRID: %06lX  Addr: %04X\nCmd: %02X (%s)\nStart: %02X  Chk: %02X\nRaw: %016llX",
        instance->generic.protocol_name,
        instance->generic.data_count_bit,
        remote_id, addr, cmd, cmd_name,
        start, chk, d);
}

static const SubGhzProtocolDecoder subghz_protocol_aok_decoder = {
    .alloc = subghz_protocol_decoder_aok_alloc,
    .free = subghz_protocol_decoder_aok_free,
    .feed = subghz_protocol_decoder_aok_feed,
    .reset = subghz_protocol_decoder_aok_reset,
    .get_hash_data = subghz_protocol_decoder_aok_get_hash_data,
    .serialize = subghz_protocol_decoder_aok_serialize,
    .deserialize = subghz_protocol_decoder_aok_deserialize,
    .get_string = subghz_protocol_decoder_aok_get_string,
};

const SubGhzProtocol subghz_protocol_aok = {
    .name = SUBGHZ_PROTOCOL_AOK_NAME,
    .type = SubGhzProtocolTypeStatic,
    .flag = SubGhzProtocolFlag_433 | SubGhzProtocolFlag_Decodable | SubGhzProtocolFlag_Save,
    .decoder = &subghz_protocol_aok_decoder,
    .encoder = NULL,
};
