#include "SimpitOutput.h"
#include "Values.h"
#include "Config.h"
#include <Arduino.h>
#include <LedControl.h>

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

///LED-matrix (MAX7219)
LedControl ledMatrix(MATRIX_DIN_PIN, MATRIX_CLK_PIN, MATRIX_CS_PIN, 1);   // pins + helderheid in Config.h

// Input-tabel: {gameValue, rij 1-8, kolom 'A'-'H'}, precies zoals de sheet.
// VAL_NONE = LED blijft uit (nog geen bron).
MaxLed MaxLeds[] = {
    // A: joystick (InputState, komt later)
    {VAL_NONE,          1, 'A'},    // J1
    {VAL_NONE,          2, 'A'},    // J2
    {VAL_NONE,          3, 'A'},    // J3
    {VAL_NONE,          4, 'A'},    // J4
    // B: mode select (InputState, komt later)
    {VAL_NONE,          1, 'B'},    // M1
    {VAL_NONE,          2, 'B'},    // M2
    {VAL_NONE,          3, 'B'},    // M3
    {VAL_NONE,          4, 'B'},    // M4
    {VAL_NONE,          5, 'B'},    // M5
    // C: arm/abort
    {VAL_NONE,          1, 'C'},    // ARM1 key switch (InputState, komt later)
    {VAL_ABORT_SIGNAL,  2, 'C'},    // ARM2 abort now!
    // D: RCS/SAS
    {VAL_AG_RCS,        1, 'D'},    // RCS
    {VAL_AG_SAS,        2, 'D'},    // SAS
    // E: timewarp 1-8
    {VAL_TW_1,          1, 'E'},    // TW1  1x
    {VAL_TW_2,          2, 'E'},    // TW2
    {VAL_TW_3,          3, 'E'},    // TW3
    {VAL_TW_4,          4, 'E'},    // TW4
    {VAL_TW_5,          5, 'E'},    // TW5
    {VAL_TW_6,          6, 'E'},    // TW6
    {VAL_TW_7,          7, 'E'},    // TW7
    {VAL_TW_8,          8, 'E'},    // TW8  100000x
    // F: timewarp 9-10 (niet in gebruik)
    {VAL_NONE,          1, 'F'},    // TW9
    {VAL_NONE,          2, 'F'},    // TW10
    // G: warnings 1-8
    {VAL_WARN_1,        1, 'G'},    // W1
    {VAL_WARN_2,        2, 'G'},    // W2
    {VAL_WARN_3,        3, 'G'},    // W3
    {VAL_WARN_4,        4, 'G'},    // W4
    {VAL_WARN_5,        5, 'G'},    // W5
    {VAL_WARN_6,        6, 'G'},    // W6
    {VAL_WARN_7,        7, 'G'},    // W7
    {VAL_WARN_8,        8, 'G'},    // W8
    // H: warnings 9-12
    {VAL_WARN_9,        1, 'H'},    // W9
    {VAL_WARN_10,       2, 'H'},    // W10
    {VAL_WARN_11,       3, 'H'},    // W11
    {VAL_WARN_12,       4, 'H'},    // W12
};

static uint8_t lastRows[8];         // wat er nu op de chip staat
static bool    rowsKnown = false;   // false → volgende ledOutput() schrijft alle rijen

//derived constants
const uint8_t numControllers = sizeof(PwmControllers) / sizeof(PwmControllers[0]);
const uint8_t numPins        = sizeof(PwmPins) / sizeof(PwmPins[0]);
const uint8_t CHANNELS_PER_BOARD = 16;
const uint8_t numMaxLeds     = sizeof(MaxLeds) / sizeof(MaxLeds[0]);

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

    //init LED-matrix
    ledMatrix.shutdown(0, false);                   // chip start in shutdown → wakker maken
    ledMatrix.setIntensity(0, MATRIX_INTENSITY);
    ledMatrix.clearDisplay(0);
    rowsKnown = false;
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

// kolom 'A'-'H' → bit in de rij-byte van LedControl.
// LedControl: bit 7 = DP, bit 6 = SEG A, bit 5 = SEG B ... bit 0 = SEG G.
// Bedrading: A-G = SEG A-G, H = DP.
static uint8_t colToMask(char col){
    if (col == 'H') return 0x80;                    // DP
    return 0x40 >> (col - 'A');                     // A → 0x40 ... G → 0x01
}

void ledOutput(){
    uint8_t rows[8] = {0};

    // rijen opbouwen vanuit de tabel
    for (uint8_t i = 0; i < numMaxLeds; i++) {
        const MaxLed &led = MaxLeds[i];
        if (led.row < 1 || led.row > 8 || led.col < 'A' || led.col > 'H') continue;   // tikfout in de tabel → overslaan
        if (getValue(led.gameValue) >= 1.0f) {
            rows[led.row - 1] |= colToMask(led.col);
        }
    }

    // alleen veranderde rijen naar de chip sturen
    for (uint8_t d = 0; d < 8; d++) {
        if (rowsKnown && rows[d] == lastRows[d]) continue;
        ledMatrix.setRow(0, d, rows[d]);
        lastRows[d] = rows[d];
    }
    rowsKnown = true;
}
