#include <Arduino.h>
#include "GameState.h"

//time
uint32_t LastTime = 0;
const int interval = 1000;
uint32_t now;

//loops

void SimpitLink(){
    if (now-LastTime >= interval) {
        game.altitude +=100;
        LastTime = now;
    }
    game.lastUpdateMs = now;
}

void GetInputs(){}
void SetControls(){}
void OutputControls(){
    Serial.print("Altitude:");
    Serial.println(game.altitude);

}

void setup()
{
    Serial.begin(9600);

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
