#include "InputState.h"
#include "Config.h"
#include <Arduino.h>

// =====================================================================
//  InputHandler: the ONLY file that touches the input pins.
//  It knows the wiring, debounces buttons, smooths and calibrates the
//  analog axes, and writes everything to `input`.
//  It does NOT know what a button does in the game: that is Controls.cpp.
// =====================================================================

InputState input = {};

// ---------------------------------------------------------------------
//  WIRING TABLE: buttons
//  Every button/switch is wired between its pin and GND.
//  The pin uses the internal pull-up, so: open = HIGH, closed = LOW.
//  Rewiring a button = changing one number here. Nothing else.
//  Pins 0/1 (Serial to KSP), 20/21 (I2C) and 51/52/53 (MAX7219) are taken.
// ---------------------------------------------------------------------
struct ButtonPin {
    ButtonID id;
    uint8_t  pin;
};

static const ButtonPin buttonPins[] = {
    // --- right edge of the shield: D2-D19 ---
    {BTN_SAS_STAB,          2},
    {BTN_SAS_PROGRADE,      3},
    {BTN_SAS_RETROGRADE,    4},
    {BTN_SAS_NORMAL,        5},
    {BTN_SAS_ANTINORMAL,    6},
    {BTN_SAS_RADIAL_IN,     7},
    {BTN_SAS_RADIAL_OUT,    8},
    {BTN_SAS_TARGET,        9},
    {BTN_SAS_ANTITARGET,   10},
    {BTN_SAS_MANEUVER,     11},
    {BTN_SAS_TOGGLE,       12},
    {BTN_RCS_TOGGLE,       13},
    {BTN_J1_BUTTON,        14},
    {BTN_J2_BUTTON,        15},
    {BTN_J1_PRECISION,     16},
    {BTN_J1_EVA,           17},
    {BTN_J2_PRECISION,     18},
    {BTN_J2_CAMERA,        19},

    // --- bottom edge of the shield: D22-D47 ---
    {BTN_FN_LIGHTS,        22},
    {BTN_FN_GEAR,          23},
    {BTN_FN_BRAKES,        24},
    {BTN_FN_SOLAR,         25},
    {BTN_FN_COMMS,         26},
    {BTN_CAG_1,            27},
    {BTN_CAG_2,            28},
    {BTN_CAG_3,            29},
    {BTN_CAG_4,            30},
    {BTN_CAG_5,            31},
    {BTN_ARM,              32},
    {BTN_STAGE,            33},
    {BTN_ABORT,            34},
    {BTN_MODE_1,           35},
    {BTN_MODE_2,           36},
    {BTN_MODE_3,           37},
    {BTN_MODE_4,           38},
    {BTN_MODE_5,           39},
    {BTN_NAV_1,            40},
    {BTN_NAV_2,            41},
    {BTN_NAV_3,            42},
    {BTN_NAV_4,            43},
    {BTN_NAV_5,            44},
    {BTN_NAV_SWITCH_1_UP,  45},
    {BTN_NAV_SWITCH_1_DOWN,46},
    {BTN_NAV_SWITCH_2_UP,  47},

    // --- left edge of the shield: D48-D50 ---
    {BTN_NAV_SWITCH_2_DOWN,48},
    // spare: D49, D50, and A7-A15 (analog pins also work as digital inputs)
};
static_assert(sizeof(buttonPins) / sizeof(buttonPins[0]) == BTN_COUNT,
              "buttonPins[] must have exactly one row per ButtonID");

// ---------------------------------------------------------------------
//  WIRING TABLE: analog axes (left edge of the shield, A0-A6)
//  centred: true for joystick axes (rest position in the middle),
//           false for the throttle (rest position at one end).
//  invert:  flip the direction if an axis turns out to work backwards.
//  rawMin / rawMax: the ADC values at the ends of the travel. Most pots
//           don't reach 0/1023 exactly; measure and fill in for full range.
// ---------------------------------------------------------------------
struct AxisPin {
    AxisID  id;
    uint8_t pin;
    bool    centred;
    bool    invert;
    int16_t rawMin;
    int16_t rawMax;
};

static const AxisPin axisPins[] = {
    //  id             pin  centred invert  rawMin rawMax
    {AXIS_J1_X,        A0,  true,   false,  0,     1023},
    {AXIS_J1_Y,        A1,  true,   false,  0,     1023},
    {AXIS_J1_Z,        A2,  true,   false,  0,     1023},
    {AXIS_J2_X,        A3,  true,   false,  0,     1023},
    {AXIS_J2_Y,        A4,  true,   false,  0,     1023},
    {AXIS_J2_Z,        A5,  true,   false,  0,     1023},
    {AXIS_THROTTLE,    A6,  false,  false,  0,     1023},   // lever down should give 0
};
static_assert(sizeof(axisPins) / sizeof(axisPins[0]) == AXIS_COUNT,
              "axisPins[] must have exactly one row per AxisID");

// ---------------------------------------------------------------------
//  Private state (only this file needs it)
// ---------------------------------------------------------------------
const uint8_t NO_PIN = 0xFF;

static uint8_t       pinOf[BTN_COUNT];          // ButtonID -> pin (built from the table)
static bool          rawLast[BTN_COUNT];        // last raw (bouncy) reading
static unsigned long rawChangedAt[BTN_COUNT];   // when that raw reading last changed

static const AxisPin* axisDef[AXIS_COUNT];      // AxisID -> table row
static float         filtered[AXIS_COUNT];      // smoothed raw ADC value
static float         centre[AXIS_COUNT];        // ADC value at rest (centred axes only)

// ---------------------------------------------------------------------
//  Helpers
// ---------------------------------------------------------------------
static bool readClosed(uint8_t pin) {
    return digitalRead(pin) == LOW;             // pull-up: LOW = contact closed
}

// smoothed raw ADC value -> -1..+1 (centred) or 0..1 (not centred)
static float scaleAxis(AxisID id) {
    const AxisPin& a = *axisDef[id];
    float f = filtered[id];
    float v;

    if (a.centred) {
        // each half is scaled separately, so the rest position is exactly 0
        // even if the centre isn't exactly in the middle of the pot
        float c = centre[id];
        if (f >= c) {
            float span = a.rawMax - c;
            v = (span > 0) ? (f - c) / span : 0.0f;
        } else {
            float span = c - a.rawMin;
            v = (span > 0) ? (f - c) / span : 0.0f;
        }
        v = constrain(v, -1.0f, 1.0f);
        if (a.invert) v = -v;
    } else {
        float span = a.rawMax - a.rawMin;
        v = (span > 0) ? (f - a.rawMin) / span : 0.0f;
        v = constrain(v, 0.0f, 1.0f);
        if (a.invert) v = 1.0f - v;
    }
    return v;
}

static void readMode() {
    input.modeChanged = false;
    for (uint8_t m = 0; m < MODE_COUNT; m++) {
        if (input.button[BTN_MODE_1 + m].held) {
            if (input.mode != (SimpitMode)m) {
                input.mode = (SimpitMode)m;
                input.modeChanged = true;
            }
            return;
        }
    }
    // no position closed (dial between two clicks): keep the last mode
}
static_assert(BTN_MODE_5 - BTN_MODE_1 + 1 == MODE_COUNT, "need one dial position per SimpitMode");

// ---------------------------------------------------------------------
//  initInputs: once, in setup()
// ---------------------------------------------------------------------
void initInputs() {
    // buttons
    for (uint8_t i = 0; i < BTN_COUNT; i++) pinOf[i] = NO_PIN;

    for (const ButtonPin& b : buttonPins) {
        pinMode(b.pin, INPUT_PULLUP);
        pinOf[b.id] = b.pin;
    }
    delay(5);   // let the pull-ups charge the wires before the first read

    unsigned long now = millis();
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        bool closed = (pinOf[i] != NO_PIN) && readClosed(pinOf[i]);
        // start with the CURRENT position, without a `pressed` edge:
        // a latched stage button or a flipped toggle at power-on does nothing.
        input.button[i].held     = closed;
        input.button[i].pressed  = false;
        input.button[i].released = false;
        rawLast[i]      = closed;
        rawChangedAt[i] = now;
    }

    // axes: calibrate the centre. HANDS OFF THE STICKS DURING POWER-ON!
    for (const AxisPin& a : axisPins) {
        axisDef[a.id] = &a;
        long sum = 0;
        for (uint8_t s = 0; s < AXIS_CALIBRATION_SAMPLES; s++) {
            sum += analogRead(a.pin);
        }
        float avg = (float)sum / AXIS_CALIBRATION_SAMPLES;
        centre[a.id]   = avg;
        filtered[a.id] = avg;
        input.axis[a.id] = scaleAxis(a.id);
    }

    // mode dial: start in orbit mode unless the dial says otherwise
    input.mode = MODE_ORBIT;
    readMode();
    input.modeChanged = true;   // let the displays know there is a (first) mode
}

// ---------------------------------------------------------------------
//  getInputs: every loop()
// ---------------------------------------------------------------------
void getInputs() {
    unsigned long now = millis();

    // ---- buttons: debounce ----
    // A new position is only accepted after the raw reading has been stable
    // for DEBOUNCE_MS. This adds 20 ms delay but never misses a press.
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        ButtonState& b = input.button[i];
        b.pressed  = false;     // edges only last one loop
        b.released = false;

        if (pinOf[i] == NO_PIN) continue;

        bool raw = readClosed(pinOf[i]);
        if (raw != rawLast[i]) {        // still bouncing: restart the timer
            rawLast[i]      = raw;
            rawChangedAt[i] = now;
            continue;
        }
        if (raw != b.held && now - rawChangedAt[i] >= DEBOUNCE_MS) {
            b.held = raw;
            if (raw) b.pressed  = true;
            else     b.released = true;
        }
    }

    // ---- axes: smooth + scale ----
    // Exponential smoothing: move 1/AXIS_SMOOTHING of the way to the new
    // reading each loop. Removes ADC jitter, costs a few loops of lag.
    for (const AxisPin& a : axisPins) {
        int raw = analogRead(a.pin);
        filtered[a.id] += (raw - filtered[a.id]) / AXIS_SMOOTHING;
        input.axis[a.id] = scaleAxis(a.id);
    }

    // ---- mode dial ----
    readMode();
}
