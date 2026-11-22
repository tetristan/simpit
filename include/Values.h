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

    VAL_COUNT       // het is de laatste, dus deze staat altijd gelijk aan d ehoeveelheid waarden in de ENUM, handig voor for-loops en andere functies die heir doorheen moeten loopen.
};

enum Kind :uint8_t {
    KIND_NONE,           //0
    KIND_DISTANCE,       //1
    KIND_TIME,           //2
    KIND_SPEED,          //3
    KIND_ANGLE,          //4
    KIND_PERCENT,        //5
    KIND_RATIO           //6
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
