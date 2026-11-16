#include "SimpitLink.h"
#include "GameState.h"
#include "KerbalSimpitMessageTypes.h"
#include "PayloadStructs.h"

#include <Arduino.h>
#include <KerbalSimpit.h>

//variables
/// connection variables
unsigned long lastHeard = 0;
unsigned long lastPing  = 0;
const unsigned long PING_INTERVAL = 1000; // ms
const unsigned long TIMEOUT       = 3000; // ms


//ksp connections
static KerbalSimpit MySimpit(Serial);
void messageHandler(byte messageType, byte msg[], byte msgSize);

const byte channels[] = {
    //Resources
    LF_MESSAGE,
    LF_STAGE_MESSAGE,
    ELECTRIC_MESSAGE,
    //Flight info
    ALTITUDE_MESSAGE,
    APSIDES_MESSAGE,
    APSIDESTIME_MESSAGE
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
                resourceMessage m = parseMessage<resourceMessage>(msg);
                game.liquidFuel_total = m.total;
                game.liquidFuel_available = m.available;
            }
            break;

        case LF_STAGE_MESSAGE:
            if (msgSize == sizeof(resourceMessage)) {
                resourceMessage m = parseMessage<resourceMessage>(msg);
                game.liquidFuelStage_total = m.total;
                game.liquidFuelStage_available = m.available;
            }
            break;

        case ELECTRIC_MESSAGE:
            if (msgSize == sizeof(resourceMessage)) {
                resourceMessage m = parseMessage<resourceMessage>(msg);
                game.electricCharge_total = m.total;
                game.electricCharge_available = m.available;
            }
            break;


        //Flight info
        case ALTITUDE_MESSAGE:
            if (msgSize == sizeof(altitudeMessage)) {
                altitudeMessage m = parseMessage<altitudeMessage>(msg);
                game.altitude_sealevel = m.sealevel;
                game.altitude_surface = m.surface;
            }
            break;

        case APSIDES_MESSAGE:
            if (msgSize == sizeof(apsidesMessage)) {
                apsidesMessage m = parseMessage<apsidesMessage>(msg);
                game.apoapsis = m.apoapsis;
                game.periapsis= m.periapsis;
            }
            break;

        case APSIDESTIME_MESSAGE:
            if (msgSize == sizeof(apsidesTimeMessage)) {
                apsidesTimeMessage m = parseMessage<apsidesTimeMessage>(msg);
                game.timeToApoapsis = m.apoapsis;
                game.timeToPeriapsis= m.periapsis;
            }
            break;
    }
}
