#pragma once
#include <stdint.h>

//voor elke soort resource
struct Resource {
    float available;   // current amount
    float total;       // max capacity
};

// apoapsis en periapsis
struct Apsis {
    float   altitude;  // APSIDES_MESSAGE      (m)
    int32_t timeTo;    // APSIDESTIME_MESSAGE  (s)
};


struct GameState{
    Resource liquidFuel;
    Resource electricCharge;
    Apsis apoapsis;
    Apsis periapsis;
    // bool connectedToGame;
    // float altitude;
    // bool SASOn;
    // uint32_t lastUpdateMs;
    // bool lightsOn;
    // bool landingGearDown;
};

extern GameState game;
