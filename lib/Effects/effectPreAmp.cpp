#include <Defs.h>
#include <Effects.h>
#include <MyUtils.h>

#include <math.h>
// // TODO : should work since c++17 flag is set
// #include "../../../../.platformio/packages/toolchain-xtensa-esp-elf/xtensa-esp-elf/include/c++/13.2.0/bits/algorithmfwd.h"

void masterEffectPreamp(State *statePointer, int16_t *sample)
{
    int32_t temporaryInt32 = round((int32_t)*sample * statePointer->masterGain);
    temporaryInt32 = clip(temporaryInt32, (int32_t)INT16_MIN, (int32_t)INT16_MAX);
    *sample = temporaryInt32;
} 
