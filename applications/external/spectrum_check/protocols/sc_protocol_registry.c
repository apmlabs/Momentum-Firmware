#include "sc_protocol_registry.h"
#include "weather/protocol_items.h"
#include "tpms/protocol_items.h"
#include "pocsag/protocol_items.h"

// Combined extra protocol registry (weather + tpms + pocsag)
static const SubGhzProtocol* const sc_extra_protocols[] = {
    // Weather Station protocols
    &ws_protocol_infactory,
    &ws_protocol_thermopro_tx4,
    &ws_protocol_nexus_th,
    &ws_protocol_gt_wt_02,
    &ws_protocol_gt_wt_03,
    &ws_protocol_acurite_606tx,
    &ws_protocol_acurite_609txc,
    &ws_protocol_acurite_986,
    &ws_protocol_lacrosse_tx,
    &ws_protocol_lacrosse_tx141thbv2,
    &ws_protocol_oregon2,
    &ws_protocol_oregon3,
    &ws_protocol_acurite_592txr,
    &ws_protocol_acurite_5n1,
    &ws_protocol_ambient_weather,
    &ws_protocol_solight_te44,
    &ws_protocol_auriol_th,
    &ws_protocol_oregon_v1,
    &ws_protocol_tx_8300,
    &ws_protocol_wendox_w6726,
    &ws_protocol_auriol_ahfl,
    &ws_protocol_kedsum_th,
    &ws_protocol_emose601x,
    &ws_protocol_bresser_3ch,
    &ws_protocol_vauno_en8822c,
    &ws_protocol_bl999,
    // TPMS protocols
    &tpms_protocol_schrader_gg4,
    &tpms_protocol_schrader_smd3ma4,
    &tpms_protocol_schrader_eg53ma4,
    &tpms_protocol_abarth_124,
    // POCSAG
    &subghz_protocol_pocsag,
};

const SubGhzProtocolRegistry sc_extra_protocol_registry = {
    .items = sc_extra_protocols,
    .size = sizeof(sc_extra_protocols) / sizeof(sc_extra_protocols[0]),
};
