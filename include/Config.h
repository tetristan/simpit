#pragma once
#include <stdint.h>
// #define FAKE_GAME

//tijd definieren
constexpr long SECONDS_PER_HOUR = 3600L;
constexpr long SECONDS_PER_DAY  = 6L * SECONDS_PER_HOUR;   // Kerbin dag is 6 uur, als ik op earth ga spelen moet dit 24 worden

//LCD zooi
constexpr unsigned long I2C_CLOCK= 400000; //clockspeed is normaal 100000, maar meeste i2c apparatuur kan wel ez 400k aan. Als I2C dingen raar doen dan dit aanpassen

constexpr unsigned long DISPLAY_INTERVAL_MS = 50; //elke 200mn displays updaten

constexpr unsigned long MAX_LINES= 4; //max hoeveelheid regels voor LCD's, blijft 4
constexpr unsigned long MAX_COLS= 20; //max hoeveelheid kolommen voor LCD's, blijft 20

//LED-matrix (MAX7219)
constexpr uint8_t MATRIX_DIN_PIN   = 51;  // MAX7219 DIN  (Mega hardware-SPI MOSI)
constexpr uint8_t MATRIX_CLK_PIN   = 52;  // MAX7219 CLK  (Mega hardware-SPI SCK)
constexpr uint8_t MATRIX_CS_PIN    = 53;  // MAX7219 LOAD/CS (Mega hardware-SPI SS)
constexpr uint8_t MATRIX_INTENSITY = 4;   // HELDERHEID van alle matrix-LED's: 0 (zwak) - 15 (fel). Hier aanpassen.

//INPUTS (pins: see the wiring tables at the top of InputHandler.cpp)
constexpr unsigned long DEBOUNCE_MS       = 20;    // a button must be stable this long before it counts
constexpr uint8_t  AXIS_SMOOTHING         = 4;     // 1 = no smoothing, higher = smoother but more lag
constexpr uint8_t  AXIS_CALIBRATION_SAMPLES = 16;  // readings averaged for the joystick centre at power-on

constexpr float JOYSTICK_DEADZONE   = 0.05f;  // 5% around the centre = 0. See docs/InputModule.md for adding a curve
constexpr float PRECISION_FACTOR    = 0.25f;  // precision switch: joystick output x 0.25
constexpr float THROTTLE_CUT        = 0.03f;  // bottom 3% of the throttle lever = exactly 0

constexpr unsigned long AXIS_SEND_INTERVAL_MS = 50;  // axes go to KSP at most 20x per second (only when changed)

constexpr uint8_t CAG_SOLAR = 6;   // custom action group behind the SOLAR function button (assign in the VAB)
constexpr uint8_t CAG_COMMS = 7;   // custom action group behind the COMMS function button
