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
