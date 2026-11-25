# include "SimpitOutput.h"
#include "Adafruit_PWMServoDriver.h"
#include "LcdOutput.h"
#include "Values.h"
# include <Arduino.h>

//enums
//variables
///hardware


PwmController PwmControllers[] = {
    {Adafruit_PWMServoDriver(0x41),0x41, 1},
    // {Adafruit_PWMServoDriver(0x42),0x42, 2},
};

///matrixes
PwmPin GaugePins[]{               //pins 0-5 op het eerste PWM bord zijn dus altijd voor deze 6 gauges
    {VAL_R_LF_S, 0, 0, 4095},
    {VAL_R_LF_S, 0, 0, 4095},
    {VAL_R_EC, 0, 0, 4095},
    {VAL_R_EC, 0, 0, 4095},
    {VAL_R_EC, 0, 0, 4095},
    {VAL_R_LF_S, 0, 0, 4095},
};

//derived constants
const uint8_t maxGauges = sizeof(GaugePins) / sizeof(GaugePins[0]);

void initOutput(){
    //init PWM drivers
    PwmControllers[0].controller.begin();
    PwmControllers[0].controller.setPWMFreq(1000);
    // PwmControllers[1].controller.begin();            //VOOR ALS DE 2E ER IS
    // PwmControllers[1].controller.setPWMFreq(1000);

}

void gaugeOutput(){
    //for-loop
    for (uint8_t g=0; g<maxGauges; g++) {
        PwmPin p = GaugePins[g];
        float val = getValue(p.gameValue)/100.0f;
        uint16_t pwm = (uint16_t)(p.valueMin + constrain(val, 0.0f, 1.0f) * (p.valueMax - p.valueMin) + 0.5f);
        PwmControllers[0].controller.setPin(g, pwm); //0 hardcoded want deze functie werkt alleen op de eerste PWM board
        //value calibreren

        //value schrijven

        // PwmPins[g]
    }
    return;
}
