#pragma once

#include <lib/subghz/registry.h>

// Combined protocol registry for Spectrum Check
// Includes: Weather Station (25) + TPMS (4) + POCSAG (1) = 30 extra protocols
extern const SubGhzProtocolRegistry sc_extra_protocol_registry;
