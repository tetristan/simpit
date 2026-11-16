#include "Config.h"
#include "FakeGame.h"
#include "GameState.h"

#ifdef FAKE_GAME

void fakeGameInit() {
    // connection
    game.connectedToGame = true;

    // resources (total = capacity, available = what's left)
    game.liquidFuel_total          = 3240.0f;   // whole vessel
    game.liquidFuel_available      = 1187.5f;
    game.liquidFuelStage_total     = 720.0f;    // current stage only
    game.liquidFuelStage_available = 263.4f;
    game.electricCharge_total      = 150.0f;
    game.electricCharge_available  = 112.5f;

    // flight info: climbing through 62 km, coasting to a 85 km Ap
    game.altitude_sealevel = 62480.0f;   // m
    game.altitude_surface  = 61915.0f;   // m, slightly less: over land
    game.apoapsis          = 85230.0f;   // m
    game.periapsis         = -412700.0f; // m, still suborbital → negative!
    game.timeToApoapsis    = 47.0f;      // s
    game.timeToPeriapsis   = 1384.0f;    // s, about 23 min
}

void fakeGameUpdate() {
    // empty for now
}

#endif
