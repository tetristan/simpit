#include "Handshake.h"

void initSimpit(KerbalSimpit &simpit) {
    while (!simpit.init()) {
        delay(100);
    }
    simpit.printToKSP("Connected", PRINT_TO_SCREEN);
}
