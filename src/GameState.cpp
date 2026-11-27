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

// zet warning n (1-12) aan of uit in game.warnings
void setWarning(uint8_t n, bool on){
    if (n < 1 || n > 12) return;
    uint16_t mask = (uint16_t)1 << (n - 1);
    if (on) game.warnings |= mask;
    else    game.warnings &= ~mask;
}

void updateWarnings(){
    // TODO: per warning de voorwaarde uitrekenen, bijv.
    // setWarning(1, game.electricCharge_total > 0 && game.electricCharge_available / game.electricCharge_total < 0.1f);
    return;
};


void updateDerived(){
    //calculate values
    updateHorizontalSpeed();
    updateTimeToImpact();
    updateStartBurnIn();
    updateResourceFractions();
    //calculate warnings
    updateWarnings();
}
