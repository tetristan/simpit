#include "Values.h"
#include "GameState.h"
#include "Config.h"
#include "FakeGame.h"

// Arrays //labels max 5 chars
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
