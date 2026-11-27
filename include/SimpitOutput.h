#pragma once
#include "Values.h"
#include <stdint.h>
#include <Adafruit_PWMServoDriver.h>

//enums
enum PinType : uint8_t {
    PIN_NONE,       // kanaal niet in gebruik (gat in de tabel)
    PIN_GAUGE,      // analoge meter: lineair van valueMin naar valueMax
    PIN_LED,        // LED: 0 = uit, 0.5 = valueMin (idle), 1 = valueMax (aan)
};

//structs
///PWM-ding en

struct PwmController{
    Adafruit_PWMServoDriver controller;
    uint8_t address;
    uint8_t id;
};

struct PwmPin{
    PinType type;           // gauge of LED, bepaalt hoe de waarde naar PWM gaat
    ValueID gameValue;      // een VALUE_ID zodat de goede waarde op de gauges gezet kan worden
    float inputMax;         // alleen voor gauges: welke game-waarde 'vol' is (100 voor procenten)
    uint16_t value;         // laatst geschreven PWM-waarde (0-4095), zodat we alleen bij verandering over I2C schrijven
    uint16_t valueMin;      //Gauges: PWM bij 0 (kalibratie).
    //LED's: idle-brightness (beschikbaar maar uit)
    uint16_t valueMax;      //Gauges: PWM bij inputMax (kalibratie).
    //LED's: max-brightness, zodat mijn ogen er niet uitbranden als het ding aanstaat.
};

// LED-matrix (MAX7219): één regel per LED, positie zoals in de Excel-sheet
struct MaxLed{
    ValueID gameValue;      // LED aan als getValue() >= 1 (idle 0.5 telt als uit)
    uint8_t row;            // 1-8  = DIG0-DIG7
    char    col;            // 'A'-'H' = SEG A-G, H = DP
};

void initOutput();
void pwmOutput();       // vervangt gaugeOutput(): doet gauges en LED's in één loop
void ledOutput();       // LED-matrix: alleen veranderde rijen worden geschreven
