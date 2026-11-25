#include "SimpitOutput.h"
#include "Values.h"
#include <Arduino.h>

//variables
///hardware

PwmController PwmControllers[] = {
    {Adafruit_PWMServoDriver(0x41), 0x41, 1},
    {Adafruit_PWMServoDriver(0x42), 0x42, 2},       // C2 - CHECK: klopt dit adres met je jumpers?
};

///matrixes
// De positie in deze tabel IS het kanaal: index 0-15 = C1, index 16-31 = C2.
// Kanaalnummers in de comments zijn 1-based, zoals in de bedradingstabel.
// {PIN_NONE} = kanaal niet in gebruik.
//   {type,      gameValue,          inputMax, value, valueMin, valueMax}
PwmPin PwmPins[] = {
    // ===== C1 (0x41) =====
    // gauges
    {PIN_GAUGE, VAL_R_LF,         100.0f,   0,     0,        4095},   // C1-1   G1
    {PIN_GAUGE, VAL_R_LF_S,         100.0f,   0,     0,        4095},   // C1-2   G2
    {PIN_GAUGE, VAL_HEAT,           100.0f,   0,     0,        4095},   // C1-3   G3
    {PIN_GAUGE, VAL_R_MONO,           100.0f,   0,     0,        4095},   // C1-4   G4
    {PIN_GAUGE, VAL_R_EC,           100.0f,   0,     0,        4095},   // C1-5   G5
    {PIN_GAUGE, VAL_COMMNET,         100.0f,   0,     0,        4095},   // C1-6   G6
    // functions (action groups)
    {PIN_LED,   VAL_AG_LIGHTS,      1.0f,     0,     80,       2500},   // C1-7   F1  Light
    {PIN_LED,   VAL_AG_GEAR,        1.0f,     0,     80,       2500},   // C1-8   F2  Gear
    {PIN_LED,   VAL_AG_BRAKES,      1.0f,     0,     80,       2500},   // C1-9   F3  Brake
    {PIN_LED,   VAL_CAG_1,          1.0f,     0,     80,       2500},   // C1-10  F4  C1 (custom AG 1)
    {PIN_LED,   VAL_CAG_2,          1.0f,     0,     80,       2500},   // C1-11  F5  C2
    {PIN_LED,   VAL_CAG_3,          1.0f,     0,     80,       2500},   // C1-12  F6  C3
    {PIN_LED,   VAL_CAG_4,          1.0f,     0,     80,       2500},   // C1-13  F7  C4
    {PIN_LED,   VAL_CAG_5,          1.0f,     0,     80,       2500},   // C1-14  F8  C5
    {PIN_LED,   VAL_CAG_6,          1.0f,     0,     80,       2500},   // C1-15  F9  C6
    {PIN_LED,   VAL_CAG_7,          1.0f,     0,     80,       2500},   // C1-16  F10 C7

    // ===== C2 (0x42) =====
    {PIN_NONE},                                                         // C2-1
    {PIN_NONE},                                                         // C2-2
    {PIN_NONE},                                                         // C2-3
    {PIN_NONE},                                                         // C2-4
    {PIN_NONE},                                                         // C2-5
    {PIN_NONE},                                                         // C2-6
    // SAS modes
    {PIN_LED,   VAL_SAS_STAB,       1.0f,     0,     80,       2500},   // C2-7   SAS1  stability assist
    {PIN_LED,   VAL_SAS_PROGRADE,   1.0f,     0,     80,       2500},   // C2-8   SAS2  prograde
    {PIN_LED,   VAL_SAS_RETROGRADE, 1.0f,     0,     80,       2500},   // C2-9   SAS3  retrograde
    {PIN_LED,   VAL_SAS_NORMAL,     1.0f,     0,     80,       2500},   // C2-10  SAS4  normal
    {PIN_LED,   VAL_SAS_ANTINORMAL, 1.0f,     0,     80,       2500},   // C2-11  SAS5  antinormal
    {PIN_LED,   VAL_SAS_RADIAL_IN,  1.0f,     0,     80,       2500},   // C2-12  SAS6  radial in
    {PIN_LED,   VAL_SAS_RADIAL_OUT, 1.0f,     0,     80,       2500},   // C2-13  SAS7  radial out
    {PIN_LED,   VAL_SAS_TARGET,     1.0f,     0,     80,       2500},   // C2-14  SAS8  target
    {PIN_LED,   VAL_SAS_ANTITARGET, 1.0f,     0,     80,       2500},   // C2-15  SAS9  anti-target
    {PIN_LED,   VAL_SAS_MANEUVER,   1.0f,     0,     80,       2500},   // C2-16  SAS10 maneuver
};
static_assert(sizeof(PwmPins) / sizeof(PwmPins[0]) <= 32, "meer kanalen dan 2 borden aankunnen");

//derived constants
const uint8_t numControllers = sizeof(PwmControllers) / sizeof(PwmControllers[0]);
const uint8_t numPins        = sizeof(PwmPins) / sizeof(PwmPins[0]);
const uint8_t CHANNELS_PER_BOARD = 16;

void initOutput(){
    //init PWM drivers
    for (uint8_t b = 0; b < numControllers; b++) {
        PwmControllers[b].controller.begin();
        PwmControllers[b].controller.setPWMFreq(1000);
    }
    // ongeldige 'laatste waarde' zodat elk kanaal de eerste keer sowieso geschreven wordt
    for (uint8_t i = 0; i < numPins; i++) {
        PwmPins[i].value = 0xFFFF;
    }
}

// rekent de game-waarde om naar een PWM-waarde (0-4095) voor dit kanaal
static uint16_t pinToPwm(const PwmPin &p){
    float val = getValue(p.gameValue);

    switch (p.type) {
        case PIN_GAUGE: {
            float frac = val / p.inputMax;          // bijv. procent → 0..1
            if (isnan(frac)) frac = 0.0f;           // constrain vangt NaN niet af
            frac = constrain(frac, 0.0f, 1.0f);
            return (uint16_t)(p.valueMin + frac * (p.valueMax - p.valueMin) + 0.5f);
        }
        case PIN_LED:
            if (val >= 1.0f) return p.valueMax;     // aan
            if (val > 0.0f)  return p.valueMin;     // idle: beschikbaar maar uit
            return 0;                               // donker: niet beschikbaar
        case PIN_NONE:
        default:
            return 0;
    }
}

void pwmOutput(){
    for (uint8_t i = 0; i < numPins; i++) {
        PwmPin &p = PwmPins[i];
        if (p.type == PIN_NONE) continue;

        uint8_t board = i / CHANNELS_PER_BOARD;
        if (board >= numControllers) break;         // bord bestaat (nog) niet → rest overslaan

        uint16_t pwm = pinToPwm(p);
        if (pwm == p.value) continue;               // niks veranderd → geen I2C-verkeer

        PwmControllers[board].controller.setPin(i % CHANNELS_PER_BOARD, pwm);
        p.value = pwm;
    }
}
