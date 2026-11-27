#include <Arduino.h>
//#include <KerbalSimpit.h> //niet nodig want alleen SimpitLink interfacet met deze
//#include "GameState.h" //wss niet nodig
#include "SimpitLink.h"
#include "LcdOutput.h"
#include "Config.h"
#include "FakeGame.h"
#include "GameState.h"
#include "initI2C.h"
#include "SimpitOutput.h"
#include "InputState.h"
#include "ControlState.h"

//loops

void OutputDisplays(){} //deze krijgt een eigen cpp die schrijft naar de displays

void setup()
{
    #ifdef FAKE_GAME

    fakeGameInit();
    initOutput();
    #endif

    Serial.begin(115200);
    initI2C(); //moet als eerste, want i2c is nodig voor de volgende functies
    initLcd();
    initOutput();
    initInputs(); //pinnen instellen + joysticks kalibreren: NIET aan de sticks zitten tijdens opstarten
    initSimpit(); //deze als laatste omdat deze bij gebrek aan handshake de rest van de loop blokkeert. Ook omdat de LCD's nodig zijn om weer te geven of de connectie is geslaagd
}

void loop()
{
    #ifdef FAKE_GAME
    printDebugToKSP();
    updateDerived();
    displayUpdate();
    #endif
    SimpitGetInfo();
    updateDerived(); //update de waarden die zelf berekend moeten worden, schrijf ze naar GameState


    getInputs();          //leest alle inputs van de simpit en schrijft ze naar InputState
    setControls();        //maakt van InputState (+ GameState) de ControlState
    SimpitSendControls(); //SimpitLink stuurt de ControlState naar de game
    displayUpdate();
    pwmOutput();
    ledOutput();
    printDebugToKSP();
}
