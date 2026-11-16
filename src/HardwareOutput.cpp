#include "HardwareOutput.h"
#include "LiquidCrystal_I2C.h"
#include "GameState.h"

#include <Arduino.h>

// hardware variables
/// alle LCD-addresses
LiquidCrystal_I2C lcds[] = {
    LiquidCrystal_I2C(0x27, 16, 2)
};
/// telt de grootte van de array
const uint8_t NUM_DISPLAYS = sizeof(lcds) / sizeof(lcds[0]);

// number formatting gedoe
/// ENUMS

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

/// STRUCT's

struct ValueDef{
    const char* label;
    Kind kind;
};

/// Arrays //labels max 5 chars
const ValueDef valueDefs[] = {
    {"",    KIND_NONE},         //VAL_NONE
    {"AP",  KIND_DISTANCE},     //AP
    {"tAP",KIND_TIME},          //T_AP
    {"PE",  KIND_TIME},     //PE
    {"tPE",  KIND_DISTANCE},    //T_PE
};
static_assert(sizeof(valueDefs) / sizeof(valueDefs[0]) == VAL_COUNT,
              "valueDefs out of sync with ValueID");


// functies

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
    }
}

bool isValid(ValueID id) {
    switch (id) {
        case VAL_AP:
        case VAL_T_AP:
            if(game.apoapsis <0){
                return false;
            };
        default:
            return true;
    }
}

/// formatting values



/// hardware

void initOutput(){
    //initialice LCD's
    for (uint8_t i =0; i< NUM_DISPLAYS; i++) {
        lcds[i].init();
        lcds[i].backlight();
        lcds[i].clear();
        lcds[i].setCursor(0,0);
        lcds[i].print("LCD: ");
        lcds[i].print(i+1);
    }
};

void DisplayOutput(){
    if(game.connectedToGame ==false){
        lcds[0].setCursor(0,0);
        lcds[0].print("waiting for KSP");
    }

    else if (game.connectedToGame ==true) {
        lcds[0].setCursor(0,0);
        lcds[0].print("Apoapsis: ");
        lcds[0].setCursor(0,1);
        lcds[0].print(game.apoapsis);
    }
};

void testOutput(){
    for (int i=1; i< VAL_COUNT; i++) {
        float v = getValue((ValueID)i);
        Serial.println("TEST");
        Serial.println(v);
        Serial.println(valueDefs[i].label);
    }
}
