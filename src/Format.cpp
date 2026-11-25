#include "Format.h"
#include "Config.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>

static void writeNumber(float value, uint8_t decimals, const char* unit, char* out, size_t outSize) {
    value = constrain(value, -99999.0f, 99999.0f);   // num kan niet overflowen
    char num[VALUE_TEXT_SIZE];
    dtostrf(value, 0, decimals, num);
    snprintf(out, outSize, "%s%s", num, unit);
} //interne helperfunctie voor de andere covert functies

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

    writeNumber(d, decimals, unit, out, outSize);
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

void formatSpeed(float mps, char* out, size_t outSize) {
    float a = fabs(mps);    // only for the comparison

    if (a < 1000.0f) {
        writeNumber(mps, 1, "m/s", out, outSize);
    } else if (a < 100000.0f) {
        writeNumber(mps, 0, "m/s", out, outSize);
    } else {
        float kms = constrain(mps / 1000.0f, -999.9f, 999.9f);
        writeNumber(kms, 1, "km/s", out, outSize);
    }
}

void formatAngle(float degrees, char* out, size_t outSize) {
    writeNumber(degrees, 1, "\xDF", out, outSize);
}

void formatPercent(float percent, char* out, size_t outSize) {
    writeNumber(percent, 0, "%", out, outSize);
}

void formatRatio(float ratio, char* out, size_t outSize) {
    writeNumber(ratio, 2, "", out, outSize);
}

void formatBool(float value, char* out, size_t outSize) {
    snprintf(out, outSize, "%s", (value >= 1.0f) ? "ON" : "OFF");   // 0.5 (idle) is ook OFF
}

void formatValue(Kind kind, float value, char* out, size_t outSize) { //functie om alles te combineren.
    out[0] = '\0';   // eerst leegmaken

    if (isnan(value) || isinf(value)) { //checken of value klopt
        snprintf(out, outSize, "---");
        return;
    }

    switch (kind) {
        case KIND_NONE:
            break;                                   // blijft leeg
        case KIND_DISTANCE:
            formatDistance(value, out, outSize);
            break;
        case KIND_TIME:
            formatTime(value, out, outSize);
            break;
        case KIND_SPEED:
            formatSpeed(value, out, outSize);
            break;
        case KIND_ANGLE:
            formatAngle(value, out, outSize);
            break;
        case KIND_PERCENT:
            formatPercent(value, out, outSize);
            break;
        case KIND_RATIO:
            formatRatio(value, out, outSize);
            break;
        case KIND_BOOL:
            formatBool(value, out, outSize);
            break;
    }
}
