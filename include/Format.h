#pragma once
#include <stddef.h>
#include "Values.h"

void formatDistance(float meters, char* out, size_t outSize);
void formatTime(float seconds, char* out, size_t outSize);
void formatValue(Kind kind, float value, char* out, size_t outSize);
