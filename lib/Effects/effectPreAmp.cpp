#include <Defs.h>
#include <Effects.h>

#include <math.h>

void masterEffectPreamp(State *statePointer, int16_t *sample)
{
    int32_t temporaryInt32 = round((int32_t)*sample * statePointer->masterGain);
    temporaryInt32 = std::clamp(temporaryInt32, (int32_t)INT16_MIN, (int32_t)INT16_MAX);
    *sample = temporaryInt32;
} 
