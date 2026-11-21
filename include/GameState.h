#pragma once
#include "PayloadStructs.h"
#include <stdint.h>

enum SimpitMode : uint8_t{MODE_ORBIT, MODE_LANDING, MODE_PLANE, MODE_SURFACE, MODE_RENDEZVOUS, MODE_COUNT};

struct GameState{
    //connection
    bool connectedToGame;
    SimpitMode mode;
    //game general

    //resources
    float liquidFuel_total;
    float liquidFuel_available;
    float liquidFuelStage_total;
    float liquidFuelStage_available;
    float electricCharge_total;
    float electricCharge_available;

    //flight-info
    float altitude_sealevel;
    float altitude_surface;
    float apoapsis;
    float periapsis;
    float timeToApoapsis;
    float timeToPeriapsis;

    // float altitude;
    // bool SASOn;
    // uint32_t lastUpdateMs;
    // bool lightsOn;
    // bool landingGearDown;
};

extern GameState game;
