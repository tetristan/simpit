#include "Format.h"
#include "Config.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

constexpr uint8_t NUM_BUF_SIZE = 11; //grootte dat de output van lengtes maximaal mag zijn. Kan getweakt worden als ik meer ruimte voor labels wil.

void formatDistance(float meters, char* out, size_t outSize){
    if (isnan(meters) || isinf(meters)) { //checkt of de value wel valid is
        snprintf(out, outSize, "---");
        return;
    }

    bool negative = (meters < 0);
    float d = fabs(meters);     //eerst de float absoluut maken, dus de sign wegstrippen.

    const char* unit;
    uint8_t decimals;

    if (d < 10000.0f){
        unit = "m";
        decimals = 0;
    }
    else if (d < 10000000.0f) {
        d = d/1000.0f;
        unit = "km";
        decimals = 1;
    }
    else if (d < 10000000000.0f) {  // < 10 000 Mm
        d = d / 1000000.0f;
        unit = "Mm";
        decimals = 1;
    }
    else {
        d = d / 1000000000.0f;
        unit = "Gm";
        decimals = 1;
    }
    if (d > 9999.9f) d = 9999.9f;

    char num[NUM_BUF_SIZE];
    dtostrf(d,0, decimals,num);
    snprintf(out, outSize, "%s%s%s", negative ? "-" : "", num, unit);
    };


void formatTime(float seconds, char* out, size_t outSize) {
    if (isnan(seconds) || isinf(seconds)) {
        snprintf(out, outSize, "---");
        return;
    }

    bool negative = (seconds < 0);
    float s = fabs(seconds);
    if (s > 2000000000.0f) s = 2000000000.0f;   // zorgen dat het in een long past
    long total = lround(s); //long ervan maken

    long days = total / SECONDS_PER_DAY;
    long rest = total % SECONDS_PER_DAY;
    long hours = rest / SECONDS_PER_HOUR;
    rest       = rest % SECONDS_PER_HOUR;
    long minutes = rest / 60;
    long secs    = rest % 60;

    const char* sign = negative ? "-" : "";

    if (days >0){
        snprintf(out, outSize, "%s%ldd %ldh", sign, days, hours);
    }
    else if (hours >0) {
        snprintf(out, outSize, "%s%ld:%02ld:%02ld", sign, hours, minutes, secs);
    } else if (minutes > 0) {
        snprintf(out, outSize, "%s%ld:%02ld", sign, minutes, secs);
    } else {
        snprintf(out, outSize, "%s%lds", sign, secs);
    }
}
