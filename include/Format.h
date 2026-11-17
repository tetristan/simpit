#pragma once
#include <stddef.h>
#include "Values.h"   // voor Kind

constexpr uint8_t VALUE_TEXT_SIZE = 11;   //grootte dat de output van getallen maximaal mag zijn (qua breedte in chars). Kan getweakt worden als ik meer ruimte voor labels wil.

void formatDistance(float meters, char* out, size_t outSize);
void formatTime(float seconds, char* out, size_t outSize);
void formatSpeed(float mps, char* out, size_t outSize);
void formatAngle(float degrees, char* out, size_t outSize);
void formatPercent(float percent, char* out, size_t outSize);
void formatRatio(float ratio, char* out, size_t outSize);
void formatValue(Kind kind, float value, char* out, size_t outSize);
