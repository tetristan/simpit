#include "SimpitLink.h"
#include "GameState.h"
#include "KerbalSimpitMessageTypes.h"
#include "PayloadStructs.h"
#include "Values.h"

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
    //Resources (all resourceMessage, need ARP in KSP1)
    LF_MESSAGE,
    LF_STAGE_MESSAGE,
    OX_MESSAGE,
    OX_STAGE_MESSAGE,
    SF_MESSAGE,
    SF_STAGE_MESSAGE,
    MONO_MESSAGE,
    XENON_GAS_MESSAGE,
    XENON_GAS_STAGE_MESSAGE,
    EVA_MESSAGE,
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

// All resource channels send the same resourceMessage, so one helper stores them.
// total/available are references to the GameState fields to fill.
static void storeResource(byte msg[], byte msgSize, float& total, float& available) {
    //check of de message wel volledig is aangekomen
    if (msgSize == sizeof(resourceMessage)) {
        resourceMessage m = parseMessage<resourceMessage>(msg);
        total = m.total;
        available = m.available;
    }
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
    lastHeard = millis(); //even noteren dat KSP verbonden is
    switch(messageType) {

        //Resources
        case LF_MESSAGE:
            storeResource(msg, msgSize, game.liquidFuel_total, game.liquidFuel_available);
            break;
        case LF_STAGE_MESSAGE:
            storeResource(msg, msgSize, game.liquidFuelStage_total, game.liquidFuelStage_available);
            break;
        case OX_MESSAGE:
            storeResource(msg, msgSize, game.oxidizer_total, game.oxidizer_available);
            break;
        case OX_STAGE_MESSAGE:
            storeResource(msg, msgSize, game.oxidizerStage_total, game.oxidizerStage_available);
            break;
        case SF_MESSAGE:
            storeResource(msg, msgSize, game.solidFuel_total, game.solidFuel_available);
            break;
        case SF_STAGE_MESSAGE:
            storeResource(msg, msgSize, game.solidFuelStage_total, game.solidFuelStage_available);
            break;
        case MONO_MESSAGE:
            storeResource(msg, msgSize, game.monopropellant_total, game.monopropellant_available);
            break;
        case XENON_GAS_MESSAGE:
            storeResource(msg, msgSize, game.xenon_total, game.xenon_available);
            break;
        case XENON_GAS_STAGE_MESSAGE:
            storeResource(msg, msgSize, game.xenonStage_total, game.xenonStage_available);
            break;
        case EVA_MESSAGE:
            storeResource(msg, msgSize, game.evaPropellant_total, game.evaPropellant_available);
            break;
        case ELECTRIC_MESSAGE:
            storeResource(msg, msgSize, game.electricCharge_total, game.electricCharge_available);
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
    MySimpit.printToKSP(String(getValue(VAL_R_SF)));
    return;
}
