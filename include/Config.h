#pragma once
#define FAKE_GAME

//tijd definieren
constexpr long SECONDS_PER_HOUR = 3600L;
constexpr long SECONDS_PER_DAY  = 6L * SECONDS_PER_HOUR;   // Kerbin dag is 6 uur, als ik op earth ga spelen moet dit 24 worden

constexpr unsigned long DISPLAY_INTERVAL_MS = 200; //elke 200mn displays updaten

//hardware
constexpr unsigned long MAX_LINES= 4; //max hoeveelheid regels voor LCD's, blijft 4
constexpr unsigned long MAX_COLS= 20; //max hoeveelheid kolommen voor LCD's, blijft 20
