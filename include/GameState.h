#pragma once
#include "PayloadStructs.h"
#include <stdint.h>

struct GameState{
    //connection
    bool connectedToGame;

    //resources
    resourceMessage liquidFuel;
    resourceMessage liquidFuelStage;
    resourceMessage electricCharge;
    resourceMessage electricChargeStage;

    //flight-info
    altitudeMessage altitude;
    apsidesMessage apsides;
    apsidesTimeMessage apsidesTime;

    // float altitude;
    // bool SASOn;
    // uint32_t lastUpdateMs;
    // bool lightsOn;
    // bool landingGearDown;
};

extern GameState game;
