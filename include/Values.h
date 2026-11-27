#pragma once
#include <stdint.h>

//ENUMS
enum ValueID : uint8_t {
    VAL_NONE,       // 0 - voor lege waarden
    VAL_AP,         // 1 - apoapsis
    VAL_T_AP,       // 2 - time to apoapsis
    VAL_PE,         // 3 - periapsis
    VAL_T_PE,       // 4 - time to periapsis
    VAL_ALT_ASL,    // 5 - altitude above sea level
    VAL_ALT_RADAR,  // 6 - altitude above terrain
    VAL_V_ORB,      // 7 - orbital velocity
    VAL_V_SRF,      // 8 - surface velocity
    VAL_V_VERT,     // 9 - vertical velocity
    VAL_IAS,        // 10 - indicated airspeed
    VAL_MACH,       // 11 - mach number
    VAL_G,          // 12 - g-force
    VAL_DV_STG,     // 13 - delta-v current stage
    VAL_DV_TOT,     // 14 - delta-v whole vessel
    VAL_BT_STG,     // 15 - burn time current stage
    VAL_BT_TOT,     // 16 - burn time whole vessel
    VAL_HDG,        // 17 - heading
    VAL_PITCH,      // 18 - pitch
    VAL_ROLL,       // 19 - roll
    VAL_HEAT,       // 20 - % of heat limit (highest part/skin)
    VAL_TGT_DIST,   // 21 - distance to target
    VAL_TGT_VEL,    // 22 - velocity relative to target
    VAL_TGT_HDG,    // 23 - heading to target
    VAL_TGT_PITCH,  // 24 - pitch to target
    VAL_TGT_VHDG,   // 25 - heading of relative velocity
    VAL_TGT_VPITCH, // 26 - pitch of relative velocity
    VAL_NODE_DV,    // 27 - delta-v of next node
    VAL_NODE_T,     // 28 - time to next node
    VAL_NODE_BT,    // 29 - burn time of next node
    VAL_NODE_DVTOT, // 30 - delta-v of all planned nodes

    VAL_NODE_START, //31 - time to start burn, kind_time
    VAL_V_HOR,      //32 - horizontal speed
    VAL_TTI,        //33 - time to impact

    VAL_R_LF,       //34 - Resource: LiquidFuel
    VAL_R_LF_S,     //35 - Resource: LiquidFuel per stage
    VAL_R_OX,       //36 - Resource: Oxidizer
    VAL_R_OX_S,     //37 - Resource: Oxidizer per stage
    VAL_R_SF,       //38 - Resource: Solid fuel
    VAL_R_SF_S,     //39 - Resource: Solid fuel per stage
    VAL_R_MONO,     //40 - Resource: monopropellant of vessel
    VAL_R_XENON,    //41 - Resource: xenon fuel
    VAL_R_XENON_S,  //42 - Resource: xenon fuel per stage
    VAL_R_EVA,      //43 - Resource: EVA propellant
    VAL_R_EC,       //44 - Resource: electric charge of vessel

    // LED-states: getValue() geeft 0 = donker (niet beschikbaar), 0.5 = idle (beschikbaar, uit), 1 = aan
    VAL_AG_SAS,     //45 - action group: SAS aan
    VAL_AG_RCS,     //46 - action group: RCS aan
    VAL_AG_GEAR,    //47 - action group: landingsgestel
    VAL_AG_LIGHTS,  //48 - action group: lampen
    VAL_AG_BRAKES,  //49 - action group: remmen
    VAL_AG_ABORT,   //50 - action group: abort

    VAL_SAS_STAB,       //51 - SAS mode: stability assist
    VAL_SAS_PROGRADE,   //52 - SAS mode: prograde
    VAL_SAS_RETROGRADE, //53 - SAS mode: retrograde
    VAL_SAS_NORMAL,     //54 - SAS mode: normal
    VAL_SAS_ANTINORMAL, //55 - SAS mode: antinormal
    VAL_SAS_RADIAL_IN,  //56 - SAS mode: radial in
    VAL_SAS_RADIAL_OUT, //57 - SAS mode: radial out
    VAL_SAS_TARGET,     //58 - SAS mode: target
    VAL_SAS_ANTITARGET, //59 - SAS mode: anti-target
    VAL_SAS_MANEUVER,   //60 - SAS mode: maneuver

    VAL_CAG_1,      //61 - custom action group 1
    VAL_CAG_2,      //62
    VAL_CAG_3,      //63
    VAL_CAG_4,      //64
    VAL_CAG_5,      //65
    VAL_CAG_6,      //66
    VAL_CAG_7,      //67
    VAL_CAG_8,      //68
    VAL_CAG_9,      //69
    VAL_CAG_10,     //70 - custom action group 10
    VAL_COMMNET,    //71 signal strength

    // warnings: getValue() geeft 1 = aan, 0 = uit. Berekend in updateWarnings() (GameState.cpp)
    VAL_WARN_1,      //72 - warning 1
    VAL_WARN_2,      //73 - warning 2
    VAL_WARN_3,      //74 - warning 3
    VAL_WARN_4,      //75 - warning 4
    VAL_WARN_5,      //76 - warning 5
    VAL_WARN_6,      //77 - warning 6
    VAL_WARN_7,      //78 - warning 7
    VAL_WARN_8,      //79 - warning 8
    VAL_WARN_9,      //80 - warning 9
    VAL_WARN_10,     //81 - warning 10
    VAL_WARN_11,     //82 - warning 11
    VAL_WARN_12,     //83 - warning 12

    VAL_ABORT_SIGNAL,   //84 - ARM2 "abort now!" (game.abortSignal), 1 = aan, 0 = uit

    // timewarp-balk (cumulatief): TWn aan als timewarpIndex >= n-1. Ruimte: TW1-TW8, atmosfeer: TW1-TW4
    VAL_TW_1,       //85 - 1x (aan zodra je in flight bent)
    VAL_TW_2,       //86
    VAL_TW_3,       //87
    VAL_TW_4,       //88
    VAL_TW_5,       //89
    VAL_TW_6,       //90
    VAL_TW_7,       //91
    VAL_TW_8,       //92 - 100000x

    VAL_COUNT       // het is de laatste, dus deze staat altijd gelijk aan d ehoeveelheid waarden in de ENUM, handig voor for-loops en andere functies die heir doorheen moeten loopen.
};

enum Kind :uint8_t {
    KIND_NONE,           //0
    KIND_DISTANCE,       //1
    KIND_TIME,           //2
    KIND_SPEED,          //3
    KIND_ANGLE,          //4
    KIND_PERCENT,        //5
    KIND_RATIO,          //6
    KIND_BOOL            //7 - ON/OFF (waarde >= 1 is ON)
};

// STRUCTS
struct ValueDef{
    const char* label;
    Kind kind;
};

//ARRAYS

extern const ValueDef valueDefs[];

//FUNCTIONS
float getValue(ValueID id);
bool isValid(ValueID id);
