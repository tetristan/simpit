#pragma once
#include <stdint.h>

//ENUMS
enum ValueID {
    VAL_NONE,       // 0 - voor lege waarden
    VAL_AP,         // 1 - apoapsis
    VAL_T_AP,       // 2 - time to apoapsis
    VAL_PE,         // 3 - periapsis
    VAL_T_PE,       // 4 - time to periapsis

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
void updateDerived();
