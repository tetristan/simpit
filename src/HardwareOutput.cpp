#include "HardwareOutput.h"
#include "LiquidCrystal_I2C.h"
#include "GameState.h"

#include <Arduino.h>

LiquidCrystal_I2C lcds[] = {
    LiquidCrystal_I2C(0x27, 16, 2)
};

const uint8_t NUM_DISPLAYS = sizeof(lcds) / sizeof(lcds[0]); //telt de grootte van de array

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
        lcds[0].print(game.apsides.apoapsis);
    }
};
