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
    //connection
    bool connectedToGame;

    //resources
    Resource liquidFuel;
    Resource electricCharge;

    //flight-info
    Apsis apoapsis;
    Apsis periapsis;

    // float altitude;
    // bool SASOn;
    // uint32_t lastUpdateMs;
    // bool lightsOn;
    // bool landingGearDown;
};

extern GameState game;
