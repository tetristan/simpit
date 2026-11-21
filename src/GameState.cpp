#include "GameState.h"
#include "PayloadStructs.h"
GameState game = {};


//zelf-berekende waarden
void updateHorizontalSpeed(){
    // float surfSpd =
    // float vertSpd =
    // =sqrt(surfspt^2 - vertical^2)
};
void updateTimeToImpact(){
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
    // float burntime (voor maneuver)
    // =burntime/2
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
