#include "SimpitLink.h"
#include "GameState.h"

#include <Arduino.h>
#include <KerbalSimpit.h>

static KerbalSimpit MySimpit(Serial);

void initSimpit() {
    while (!MySimpit.init()) {
        delay(100);
    }
    MySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
    game.connectedToGame = true;
}

void SimpitGetInfo(){};
