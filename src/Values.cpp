#include "Values.h"
#include "GameState.h"
#include "Config.h"
#include "FakeGame.h"

// Arrays //labels max 5 chars
const ValueDef valueDefs[] = {
    {"",    KIND_NONE},         //VAL_NONE
    {"AP",  KIND_DISTANCE},     //AP
    {"tAP",KIND_TIME},          //T_AP
    {"PE",  KIND_DISTANCE},     //PE
    {"tPE",  KIND_TIME},    //T_PE
    {"ASL",   KIND_DISTANCE},   //ALT_ASL
    {"Radar", KIND_DISTANCE},   //ALT_RADAR
    {"V-Orb", KIND_SPEED},      //V_ORB
    {"V-Srf", KIND_SPEED},      //V_SRF
    {"V-Vrt", KIND_SPEED},      //V_VERT
    {"IAS",   KIND_SPEED},      //IAS
    {"Mach",  KIND_RATIO},      //MACH
    {"G",     KIND_RATIO},      //G
    {"StgDV", KIND_SPEED},      //DV_STG
    {"TotDV", KIND_SPEED},      //DV_TOT
    {"StgBT", KIND_TIME},       //BT_STG
    {"TotBT", KIND_TIME},       //BT_TOT
    {"Hdg",   KIND_ANGLE},      //HDG
    {"Pitch", KIND_ANGLE},      //PITCH
    {"Roll",  KIND_ANGLE},      //ROLL
    {"Heat",  KIND_PERCENT},    //HEAT
    {"Tgt",   KIND_DISTANCE},   //TGT_DIST
    {"RelV",  KIND_SPEED},      //TGT_VEL
    {"TgtH",  KIND_ANGLE},      //TGT_HDG
    {"TgtP",  KIND_ANGLE},      //TGT_PITCH
    {"RvH",   KIND_ANGLE},      //TGT_VHDG
    {"RvP",   KIND_ANGLE},      //TGT_VPITCH
    {"NdDV",  KIND_SPEED},      //NODE_DV
    {"tNode", KIND_TIME},       //NODE_T
    {"NdBT",  KIND_TIME},       //NODE_BT
    {"NdTot", KIND_SPEED},      //NODE_DVTOT

    {"t-brn", KIND_TIME},       //NODE_START
    {"V-Hor", KIND_SPEED},      //V_HOR
    {"TTI",   KIND_TIME},       //TTI (not computed yet, isValid = false)

    {"LF",    KIND_PERCENT},    //R_LF
    {"LF-S",  KIND_PERCENT},    //R_LF_S
    {"Ox",    KIND_PERCENT},    //R_OX
    {"Ox-S",  KIND_PERCENT},    //R_OX_S
    {"SF",    KIND_PERCENT},    //R_SF
    {"SF-S",  KIND_PERCENT},    //R_SF_S
    {"Mono",  KIND_PERCENT},    //R_MONO
    {"Xe",    KIND_PERCENT},    //R_XENON
    {"Xe-S",  KIND_PERCENT},    //R_XENON_S
    {"EVA",   KIND_PERCENT},    //R_EVA
    {"EC",    KIND_PERCENT},    //R_EC
};
static_assert(sizeof(valueDefs) / sizeof(valueDefs[0]) == VAL_COUNT,
              "valueDefs out of sync with ValueID");

// functies

// fill level in % (0-100). Returns 0 for an empty/absent tank; isValid() catches that case.
static float resourcePercent(float available, float total){
    if (total <= 0.0f) return 0.0f;
    return available / total * 100.0f;
}

float getValue(ValueID id){
    switch (id) {
        case VAL_NONE:
            return 0.0f;
        case VAL_AP:
            return game.apoapsis;
        case VAL_T_AP:
            return game.timeToApoapsis;
        case VAL_PE:
            return game.periapsis;
        case VAL_T_PE:
            return game.timeToPeriapsis;
        case VAL_ALT_ASL:
            return game.altitude_sealevel;
        case VAL_ALT_RADAR:
            return game.altitude_surface;
        case VAL_V_ORB:
            return game.velocity_orbital;
        case VAL_V_SRF:
            return game.velocity_surface;
        case VAL_V_VERT:
            return game.velocity_vertical;
        case VAL_IAS:
            return game.airspeed_IAS;
        case VAL_MACH:
            return game.airspeed_mach;
        case VAL_G:
            return game.gForce;
        case VAL_DV_STG:
            return game.deltaV_stage;
        case VAL_DV_TOT:
            return game.deltaV_total;
        case VAL_BT_STG:
            return game.burnTime_stage;
        case VAL_BT_TOT:
            return game.burnTime_total;
        case VAL_HDG:
            return game.heading;
        case VAL_PITCH:
            return game.pitch;
        case VAL_ROLL:
            return game.roll;
        case VAL_HEAT:
            return game.heatLimitPercent;
        case VAL_TGT_DIST:
            return game.target_distance;
        case VAL_TGT_VEL:
            return game.target_velocity;
        case VAL_TGT_HDG:
            return game.target_heading;
        case VAL_TGT_PITCH:
            return game.target_pitch;
        case VAL_TGT_VHDG:
            return game.target_velocityHeading;
        case VAL_TGT_VPITCH:
            return game.target_velocityPitch;
        case VAL_NODE_DV:
            return game.node_deltaV;
        case VAL_NODE_T:
            return game.node_timeTo;
        case VAL_NODE_BT:
            return game.node_burnTime;
        case VAL_NODE_DVTOT:
            return game.node_deltaVTotal;
        case VAL_NODE_START:
            return game.node_startBurnIn;
        case VAL_V_HOR:
            return game.velocity_horizontal;
        case VAL_TTI:
            return 0.0f;    // TODO: updateTimeToImpact() isn't written yet

            //resources: fill level in %
        case VAL_R_LF:
            return resourcePercent(game.liquidFuel_available, game.liquidFuel_total);
        case VAL_R_LF_S:
            return resourcePercent(game.liquidFuelStage_available, game.liquidFuelStage_total);
        case VAL_R_OX:
            return resourcePercent(game.oxidizer_available, game.oxidizer_total);
        case VAL_R_OX_S:
            return resourcePercent(game.oxidizerStage_available, game.oxidizerStage_total);
        case VAL_R_SF:
            return resourcePercent(game.solidFuel_available, game.solidFuel_total);
        case VAL_R_SF_S:
            return resourcePercent(game.solidFuelStage_available, game.solidFuelStage_total);
        case VAL_R_MONO:
            return resourcePercent(game.monopropellant_available, game.monopropellant_total);
        case VAL_R_XENON:
            return resourcePercent(game.xenon_available, game.xenon_total);
        case VAL_R_XENON_S:
            return resourcePercent(game.xenonStage_available, game.xenonStage_total);
        case VAL_R_EVA:
            return resourcePercent(game.evaPropellant_available, game.evaPropellant_total);
        case VAL_R_EC:
            return resourcePercent(game.electricCharge_available, game.electricCharge_total);

        case VAL_COUNT:
            return 0.0f;
    }
    return 0.0f;
}

bool isValid(ValueID id) {
    switch (id) {
        // target values: only meaningful with a target.
        // These must stay ABOVE the VAL_AP block, because that block falls through into default.
        case VAL_TGT_DIST:      //fallthrough
        case VAL_TGT_VEL:
        case VAL_TGT_HDG:
        case VAL_TGT_PITCH:
        case VAL_TGT_VHDG:
        case VAL_TGT_VPITCH:
            return game.hasTarget;

            // not computed yet
        case VAL_TTI:
            return false;

            // resources: only valid if the vessel/stage actually has that resource
        case VAL_R_LF:      return game.liquidFuel_total > 0.0f;
        case VAL_R_LF_S:    return game.liquidFuelStage_total > 0.0f;
        case VAL_R_OX:      return game.oxidizer_total > 0.0f;
        case VAL_R_OX_S:    return game.oxidizerStage_total > 0.0f;
        case VAL_R_SF:      return game.solidFuel_total > 0.0f;
        case VAL_R_SF_S:    return game.solidFuelStage_total > 0.0f;
        case VAL_R_MONO:    return game.monopropellant_total > 0.0f;
        case VAL_R_XENON:   return game.xenon_total > 0.0f;
        case VAL_R_XENON_S: return game.xenonStage_total > 0.0f;
        case VAL_R_EVA:     return game.evaPropellant_total > 0.0f;
        case VAL_R_EC:      return game.electricCharge_total > 0.0f;

        case VAL_AP:           //fallthrough
        case VAL_T_AP:
            if(game.apoapsis <0){
                return false;
            };
        default:
            return true;
    }
}
