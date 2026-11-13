#include "SimpitLink.h"
#include "GameState.h"
#include "KerbalSimpitMessageTypes.h"
#include "PayloadStructs.h"

#include <Arduino.h>
#include <KerbalSimpit.h>

//variables
unsigned long lastHeard = 0;
unsigned long lastPing  = 0;
const unsigned long PING_INTERVAL = 1000; // ms
const unsigned long TIMEOUT       = 3000; // ms


//ksp connections
static KerbalSimpit MySimpit(Serial);
void messageHandler(byte messageType, byte msg[], byte msgSize);

const byte channels[] = {
    ALTITUDE_MESSAGE,
    VELOCITY_MESSAGE,
    APSIDES_MESSAGE,
    LF_MESSAGE,
    OX_MESSAGE
};

void registerAllChannels() {
    for (byte ch : channels) {
        MySimpit.registerChannel(ch);
    }
}

void initSimpit() {
    game.connectedToGame =false;
    MySimpit.inboundHandler(messageHandler);

}

void tryConnect() {
    if (MySimpit.init()) {
        game.connectedToGame = true;
        lastHeard = millis();
        MySimpit.printToKSP("Reonnected hihi", PRINT_TO_SCREEN);
        registerAllChannels();   // opnieuw subben aan channels omdat ksp vergeet na een disconnect
    }
}



void SimpitGetInfo(){
    if(game.connectedToGame == false){
        tryConnect();
        return;
    }

    MySimpit.update();

    if (millis() - lastPing > PING_INTERVAL) {
        MySimpit.send(ECHO_REQ_MESSAGE, "p", 1);
        lastPing = millis();
    }

    if (millis() - lastHeard > TIMEOUT) {
        game.connectedToGame = false;   // verbinding verbroken, start handshake
    }


};

void messageHandler(byte messageType, byte msg[], byte msgSize) {
    lastHeard = millis(); //even noteren dat KSP verbonden is
    switch(messageType) {

        //Resources
        case LF_MESSAGE:
            //check of de message wel volledig is aangekomen
            if (msgSize == sizeof(resourceMessage)) {
                game.liquidFuel = parseMessage<resourceMessage>(msg);
            }
            break;

        case LF_STAGE_MESSAGE:
            //check of de message wel volledig is aangekomen
            if (msgSize == sizeof(resourceMessage)) {
                game.liquidFuelStage = parseMessage<resourceMessage>(msg);
            }
            break;

        case ELECTRIC_MESSAGE:
            //check of de message wel volledig is aangekomen
            if (msgSize == sizeof(resourceMessage)) {
                game.electricCharge = parseMessage<resourceMessage>(msg);
            }
            break;


        //Flight info
        case ALTITUDE_MESSAGE:
            //check of de message wel volledig is aangekomen
            if (msgSize == sizeof(altitudeMessage)) {
                game.altitude = parseMessage<altitudeMessage>(msg);
            }
            break;

        case APSIDES_MESSAGE:
            //check of de message wel volledig is aangekomen
            if (msgSize == sizeof(apsidesMessage)) {
                game.apsides = parseMessage<apsidesMessage>(msg);
            }
            break;

        case APSIDESTIME_MESSAGE:
            //check of de message wel volledig is aangekomen
            if (msgSize == sizeof(apsidesTimeMessage)) {
                game.apsidesTime = parseMessage<apsidesTimeMessage>(msg);
            }
            break;
    }
}
