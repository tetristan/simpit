#pragma once
#include "PayloadStructs.h"
#include <stdint.h>
#include "Values.h"

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
    float oxidizer_total;
    float oxidizer_available;
    float oxidizerStage_total;
    float oxidizerStage_available;
    float solidFuel_total;
    float solidFuel_available;
    float solidFuelStage_total;
    float solidFuelStage_available;
    float xenon_total;
    float xenon_available;
    float xenonStage_total;
    float xenonStage_available;
    float electricCharge_total;
    float electricCharge_available;
    float monopropellant_total;     // monopropellant of the vessel
    float monopropellant_available; // ^
    float evaPropellant_total;      // only while on EVA (KSP1)
    float evaPropellant_available;  // ^

    //flight-info
    float altitude_sealevel;
    float altitude_surface;
    float apoapsis;
    float periapsis;
    float timeToApoapsis;
    float timeToPeriapsis;

    //velocity
    float velocity_orbital;
    float velocity_surface;
    float velocity_vertical;

    //airspeed
    float airspeed_IAS;
    float airspeed_mach;
    float gForce;

    //deltaV and burn time
    float deltaV_stage;
    float deltaV_total;
    float burnTime_stage;
    float burnTime_total;

    //rotation (vessel pointing)
    float heading;      // 0 - 360
    float pitch;        // -90 - +90
    float roll;         // -180 - +180

    //temperature
    float heatLimitPercent;   // 0 - 100, highest of part temp and skin temp

    //flight status (flags and status bytes keep their own types)
    bool inFlight;
    bool isEVA;
    bool hasTarget;
    uint8_t situation;          // KSP1 Vessel.Situations flags: 1 landed, 2 splashed, 4 prelaunch, 8 flying, 16 suborbital, 32 orbiting, 64 escaping, 128 docked
    float currentStage;         // number of stages remaining (capped at 255)
    float commNetSignalPercent; // 0 - 100, 0 when CommNet isn't used

    //target (only meaningful when hasTarget)
    float target_distance;
    float target_velocity;          // relative velocity
    float target_heading;           // direction to the target
    float target_pitch;
    float target_velocityHeading;   // direction of the relative velocity
    float target_velocityPitch;

    //maneuver node (no "has node" flag yet: test in KSP what arrives without a node)
    float node_timeTo;
    float node_deltaV;
    float node_burnTime;
    float node_deltaVTotal;     // all planned nodes together
    float node_heading;
    float node_pitch;

    float node_startBurnIn;     // zelf berekenen
    float velocity_horizontal;     // zelf berekenen

    uint8_t actionGroups = 0;   // bitmask: SAS_ACTION, RCS_ACTION, ...
    uint8_t sasMode      = 255; // 255 = SAS uit. NIET 0, want 0 = stability assist
    int16_t sasAvailable = 0;   // bitmask: 1 << AP_PROGRADE enz.
    uint8_t cag[2]       = {0, 0};

    // float altitude;
    // bool SASOn;
    // uint32_t lastUpdateMs;
    // bool lightsOn;
    // bool landingGearDown;
};

extern GameState game;

void updateDerived();
