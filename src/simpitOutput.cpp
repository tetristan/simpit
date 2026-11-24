# include "SimpitOutput.h"
#include "LcdOutput.h"
#include "Values.h"
# include <Arduino.h>

//enums

//variables
///hardware
const uint8_t maxGauges = 6;

///matrixes
PwmPin PwmPins[]{
    PwmPin{VAL_R_SF, 0, 0, 4095},
    PwmPin{VAL_R_SF_S, 0, 0, 4095},
    PwmPin{VAL_R_EC, 0, 0, 4095},
    PwmPin{VAL_R_EC, 0, 0, 4095},
    PwmPin{VAL_R_EC, 0, 0, 4095},
    PwmPin{VAL_R_EC, 0, 0, 4095},
};

void initOutput(){
    return;
}

void gaugeOutput(){
    //for-loop
    for (uint8_t g=0; g<maxGauges; g++) {

        return;
    }
    return;
}
