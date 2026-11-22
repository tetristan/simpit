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
    APSIDESTIME_MESSAGE,
    VELOCITY_MESSAGE,
    AIRSPEED_MESSAGE,
    ROTATION_DATA_MESSAGE,
    //Vessel info
    DELTAV_MESSAGE,
    BURNTIME_MESSAGE,
    TEMP_LIMIT_MESSAGE,
    FLIGHT_STATUS_MESSAGE,
    //Navigation
    TARGETINFO_MESSAGE,
    MANEUVER_MESSAGE
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

        case VELOCITY_MESSAGE:
            if (msgSize == sizeof(velocityMessage)) {
                velocityMessage m = parseMessage<velocityMessage>(msg);
                game.velocity_orbital = m.orbital;
                game.velocity_surface = m.surface;
                game.velocity_vertical = m.vertical;
            }
            break;

        case AIRSPEED_MESSAGE:
            if (msgSize == sizeof(airspeedMessage)) {
                airspeedMessage m = parseMessage<airspeedMessage>(msg);
                game.airspeed_IAS = m.IAS;
                game.airspeed_mach = m.mach;
                game.gForce = m.gForces;
            }
            break;

        case ROTATION_DATA_MESSAGE:
            if (msgSize == sizeof(vesselPointingMessage)) {
                vesselPointingMessage m = parseMessage<vesselPointingMessage>(msg);
                game.heading = m.heading;
                game.pitch = m.pitch;
                game.roll = m.roll;
            }
            break;

            //Vessel info
        case DELTAV_MESSAGE:
            if (msgSize == sizeof(deltaVMessage)) {
                deltaVMessage m = parseMessage<deltaVMessage>(msg);
                game.deltaV_stage = m.stageDeltaV;
                game.deltaV_total = m.totalDeltaV;
            }
            break;

        case BURNTIME_MESSAGE:
            if (msgSize == sizeof(burnTimeMessage)) {
                burnTimeMessage m = parseMessage<burnTimeMessage>(msg);
                game.burnTime_stage = m.stageBurnTime;
                game.burnTime_total = m.totalBurnTime;
            }
            break;

        case TEMP_LIMIT_MESSAGE:
            if (msgSize == sizeof(tempLimitMessage)) {
                tempLimitMessage m = parseMessage<tempLimitMessage>(msg);
                // show whichever is closer to its limit: part temp or skin temp
                byte highest = (m.tempLimitPercentage > m.skinTempLimitPercentage)
                ? m.tempLimitPercentage
                : m.skinTempLimitPercentage;
                game.heatLimitPercent = (float)highest;
            }
            break;

        case FLIGHT_STATUS_MESSAGE:
            if (msgSize == sizeof(flightStatusMessage)) {
                flightStatusMessage m = parseMessage<flightStatusMessage>(msg);
                game.inFlight = m.isInFlight();
                game.isEVA = m.isInEVA();
                game.hasTarget = m.hasTarget();
                game.situation = m.vesselSituation;
                game.currentStage = (float)m.currentStage;
                game.commNetSignalPercent = (float)m.commNetSignalStrenghPercentage; // typo is in the library
            }
            break;

            //Navigation
        case TARGETINFO_MESSAGE:
            if (msgSize == sizeof(targetMessage)) {
                targetMessage m = parseMessage<targetMessage>(msg);
                game.target_distance = m.distance;
                game.target_velocity = m.velocity;
                game.target_heading = m.heading;
                game.target_pitch = m.pitch;
                game.target_velocityHeading = m.velocityHeading;
                game.target_velocityPitch = m.velocityPitch;
            }
            break;

        case MANEUVER_MESSAGE:
            if (msgSize == sizeof(maneuverMessage)) {
                maneuverMessage m = parseMessage<maneuverMessage>(msg);
                game.node_timeTo = m.timeToNextManeuver;
                game.node_deltaV = m.deltaVNextManeuver;
                game.node_burnTime = m.durationNextManeuver;
                game.node_deltaVTotal = m.deltaVTotal;
                game.node_heading = m.headingNextManeuver;
                game.node_pitch = m.pitchNextManeuver;
            }
            break;
    }
}

//debug functions

void printDebugToKSP(){
    MySimpit.printToKSP(String("NODE INFO"));
    MySimpit.printToKSP(String(game.node_deltaV));
    return;
}
