#include "HardwareOutput.h"
#include "LiquidCrystal_I2C.h"
#include "GameState.h"
#include "Values.h"

#include <Arduino.h>

// hardware variables
/// alle LCD-addresses
LiquidCrystal_I2C lcds[] = {
    LiquidCrystal_I2C(0x27, 16, 2)
};
/// telt de grootte van de array
const uint8_t NUM_DISPLAYS = sizeof(lcds) / sizeof(lcds[0]);

// functions

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
