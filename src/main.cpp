#include <Arduino.h>
#include <KerbalSimpit.h>
#include "GameState.h"
#include "Handshake.h"

//time
uint32_t LastTime = 0;
const int interval = 1000;
uint32_t now;

//simpit reference
KerbalSimpit mySimpit(Serial);

//loops

void SimpitLink(){}
void GetInputs(){}
void SetControls(){}
void OutputControls(){}

void setup()
{
    Serial.begin(115200);
    initSimpit(mySimpit);
}

void loop()
{
    now = millis();
    SimpitLink();
    GetInputs();
    SetControls();
    OutputControls();

    // now = millis();
    // if (now-LastTime >= interval) {
    //     Serial.println("tick");
    //     Serial.println(now);
    //     LastTime = now;
    // }
}
