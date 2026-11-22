#include "GameState.h"
#include "PayloadStructs.h"
GameState game = {};


//zelf-berekende waarden
void updateHorizontalSpeed(){
    float vs = game.velocity_surface;
    float vv = game.velocity_vertical;
    game.velocity_horizontal = sqrt(max(0.0f, vs*vs - vv*vv));
    return;
};
void updateTimeToImpact(){
    // float vDown = -game.velocity_vertical;
    // float h = game.altitude_surface;

    // float vertSpd
    // float radar-alt
    // if vertSpd >0 (als vertSpd 0 is, maak de return 'NAN', dan maakt IsValid er --- van).
    // radar-alt/vertSpd

    //alternatieve betere formule:
    // - zwaartekracht meerekenen, die versnelt naarmate ik dichterbij kom
    // - trajectory is niet recht
    // oplossing: een tabel maken per met per BODY de g en de radius. Dan via SOI message weten welke body ik omheen ben, en voila :)
    return;
};
void updateStartBurnIn(){
    game.node_startBurnIn = game.node_timeTo - (game.node_burnTime / 2.0f); //want de burn vind precies halverwege de node plaats (de helft voor en de helft na de node).
    return;

};
void updateResourceFractions(){
    //float available
    //float total

    //deze wss gewoon bij de gauge-functie meenemen, is toch alleen daarvoor nodig
    return;

};


void updateDerived(){
    updateHorizontalSpeed();
    updateTimeToImpact();
    updateStartBurnIn();
    updateResourceFractions();
}
