#pragma once
#include "PayloadStructs.h"
#include <stdint.h>

struct GameState{
    //connection
    bool connectedToGame;

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
