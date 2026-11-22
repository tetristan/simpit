# include "initI2C.h"
#include "Config.h"
# include <Wire.h>

//hier de I2C connectie beginnen

void initI2C(){
    Wire.begin();
    Wire.setWireTimeout();
    Wire.setClock(I2C_CLOCK);
}
