#include <Arduino.h>
//#include <KerbalSimpit.h> //niet nodig want alleen SimpitLink interfacet met deze
//#include "GameState.h" //wss niet nodig
#include "SimpitLink.h"
#include "HardwareOutput.h"
#include "Config.h"
#include "FakeGame.h"

//loops

void GetInputs(){}
void SetControls(){}
void OutputControls(){} //deze gaat naar SimpitLink
void OutputDisplays(){} //deze krijgt een eigen cpp die schrijft naar de displays

void setup()
{
#ifdef FAKE_GAME
    Serial.begin(9600);

    fakeGameInit();
    initOutput();
    testOutput();


#else
    Serial.begin(115200);
    initOutput();
    initSimpit(); //deze als laatste omdat deze bij gebrek aan handshake de rest van de loop blokkeert. Ook omdat de LCD's nodig zijn om weer te geven of de connectie is geslaagd

#endif
}

void loop()
{
#ifdef FAKE_GAME
    DisplayOutput();
#else
    SimpitGetInfo();

    //deze komen nog
    GetInputs(); //hier leest ie alle inputs van de simpit en schrijft ze naar een openbare struct
    SetControls(); //hier vergelijkt ie de inputs met de gamestate, en maakt hij de nieuwe Controlstate
    OutputControls(); //hier roept ie de SimpitLink om de Controlstate naar de game te schrijven, en om de gauges en displays aan te drijven (al split ik die miss later nog)

    DisplayOutput();
#endif
}
