#include "SimpitLink.h"
#include "GameState.h"
#include "KerbalSimpitMessageTypes.h"
#include "PayloadStructs.h"

#include <KerbalSimpit.h>

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
    while (!MySimpit.init()) {
        delay(100);
    }
    MySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
    game.connectedToGame = true;

    //nu subscriben op alle channels
    MySimpit.inboundHandler(messageHandler);

}

void SimpitGetInfo(){
    MySimpit.update();
};

void messageHandler(byte messageType, byte msg[], byte msgSize) {
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
