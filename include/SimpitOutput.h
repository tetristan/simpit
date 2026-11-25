#pragma once
# include "Values.h"
#include <stdint.h>
#include <Adafruit_PWMServoDriver.h>

//enums

// enum GaugeID uint8_t{
//     GAUGE_LF,
//     GAUGE_LF_S,
//     GAUGE_X
//     GAUGE_HEAT,
// };

//structs
///PWM-ding en

struct PwmController{
    Adafruit_PWMServoDriver controller;
    uint8_t address;
    uint8_t id;
};

struct PwmPin{
    ValueID gameValue;      // een VALUE_ID zodat de goede waarde op de gauges gezet kan worden
    uint16_t value;         // waarde tussen 0 en 4095, die de PWM controller toe kan wijzen. Voor de gauges wordt dit via value geregeld, maar voor LED's is dit de brightness
    uint16_t valueMin;      //minimale waarde die de LED mag worden.
                                //Voor gauges interpolate hij dit om volle range te krijgen.
                                //Voor LED's kan dit een brightness-value zijn en een min voor als ze 'idle' staan
    uint16_t valueMax;      //minimale waarde die de LED mag worden.
                                //Voor gauges interpolate hij dit om volle range te krijgen.
                                //Voor LED's kan dit een max-brightness-value zijn, zodat mijn ogen er niet uitbranden als het ding aanstaat.
};

void initOutput();
void ledOutput();
void gaugeOutput();
