#include "Config.h"
#include "FakeGame.h"
#include "GameState.h"
#include "KerbalSimpitMessageTypes.h"

#ifdef FAKE_GAME

void fakeGameInit() {
    // connection
    game.connectedToGame = true;
    game.mode = MODE_ORBIT;

    // resources (total = capacity, available = what's left)
    game.liquidFuel_total          = 3240.0f;   // whole vessel
    game.liquidFuel_available      = 1187.5f;
    game.liquidFuelStage_total     = 720.0f;    // current stage only
    game.liquidFuelStage_available = 263.4f;
    game.oxidizer_total            = 3960.0f;   // LF:Ox is 9:11 in KSP
    game.oxidizer_available        = 1451.4f;
    game.oxidizerStage_total       = 880.0f;
    game.oxidizerStage_available   = 321.9f;
    game.solidFuel_total           = 0.0f;      // boosters already dropped → tests "---"
    game.solidFuel_available       = 0.0f;
    game.solidFuelStage_total      = 0.0f;
    game.solidFuelStage_available  = 0.0f;
    game.monopropellant_total      = 120.0f;
    game.monopropellant_available  = 9.6f;      // 8%: tests a nearly empty gauge
    game.xenon_total               = 0.0f;      // no ion engine → tests "---"
    game.xenon_available           = 0.0f;
    game.xenonStage_total          = 0.0f;
    game.xenonStage_available      = 0.0f;
    game.evaPropellant_total       = 0.0f;      // not on EVA → tests "---"
    game.evaPropellant_available   = 0.0f;
    game.electricCharge_total      = 150.0f;
    game.electricCharge_available  = 112.5f;    // 75%

    // flight info: climbing through 62 km, coasting to a 85 km Ap
    game.altitude_sealevel = 62480.0f;   // m
    game.altitude_surface  = 61915.0f;   // m, slightly less: over land
    game.apoapsis          = 85230.0f;   // m
    game.periapsis         = -412700.0f; // m, still suborbital → negative!
    game.timeToApoapsis    = 47.0f;      // s
    game.timeToPeriapsis   = 1384.0f;    // s, about 23 min

    // velocity (orbital is higher than surface because of Kerbin's rotation)
    game.velocity_orbital  = 1820.0f;    // m/s
    game.velocity_surface  = 1650.0f;    // m/s
    game.velocity_vertical = 470.0f;     // m/s, still climbing

    // airspeed: very thin air at 62 km
    game.airspeed_IAS  = 38.5f;          // m/s
    game.airspeed_mach = 5.62f;
    game.gForce        = 0.15f;          // coasting

    // delta-v and burn time
    game.deltaV_stage   = 1240.0f;       // m/s
    game.deltaV_total   = 3050.0f;       // m/s
    game.burnTime_stage = 38.0f;         // s
    game.burnTime_total = 212.0f;        // s, 3:32

    // rotation: heading east, nose slightly up, small negative roll
    game.heading = 90.0f;
    game.pitch   = 18.4f;
    game.roll    = -2.5f;                // tests a negative angle

    // heat
    game.heatLimitPercent = 34.0f;

    // flight status
    game.inFlight             = true;
    game.isEVA                = false;
    game.hasTarget            = true;    // set to false to test the "---" lines
    game.situation            = 16;      // KSP1: suborbital
    game.currentStage         = 2.0f;    // 2 stages left
    game.commNetSignalPercent = 100.0f;

    // target: a station in a higher orbit
    game.target_distance        = 312400.0f; // m
    game.target_velocity        = 845.0f;    // m/s
    game.target_heading         = 92.3f;
    game.target_pitch           = 4.1f;
    game.target_velocityHeading = 271.5f;
    game.target_velocityPitch   = -3.2f;     // tests a negative angle

    // maneuver node: circularisation burn at Ap
    game.node_timeTo      = 47.0f;       // s, same as time to Ap
    game.node_deltaV      = 682.0f;      // m/s
    game.node_burnTime    = 41.0f;       // s
    game.node_deltaVTotal = 682.0f;      // m/s, only one node
    game.node_heading     = 90.0f;
    game.node_pitch       = 0.0f;

    // buttons/LEDs: SAS + lampen aan, rest idle
    game.actionGroups = SAS_ACTION | LIGHT_ACTION;
    game.sasMode      = AP_PROGRADE;
    // level-2 piloot: geen target/anti-target/maneuver → test de donkere LED's
    game.sasAvailable = (1 << AP_STABILITYASSIST) | (1 << AP_PROGRADE) | (1 << AP_RETROGRADE)
    | (1 << AP_NORMAL) | (1 << AP_ANTINORMAL)
    | (1 << AP_RADIALIN) | (1 << AP_RADIALOUT);
    game.cag[0] = (1 << 1) | (1 << 3);   // AG1 en AG3 aan
    game.cag[1] = 0;
}

void fakeGameUpdate() {
    // empty for now
}

#endif
