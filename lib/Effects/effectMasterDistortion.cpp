#include <Defs.h>
#include <Effects.h>

#include <math.h>

void masterEffectDistortion(int16_t *sample)
{

    int32_t q = 1.5;
    int32_t temporaryInt32 = std::clamp((int32_t)*sample * q, (int32_t)INT16_MIN, (int32_t)INT16_MAX);

    // // float_t q = 1.2;
    // int32_t range = -INT16_MIN + INT16_MAX;
    // int32_t x = (*sample + range / 2) / range;
    // int32_t temporaryInt32 = ((x * x * (3.0 - 2.0 * x)) - 0.5) * range;

    // float q = 1.2;
    // int32_t temporaryInt32 = (((-INT16_MIN + INT16_MAX) / 2) / (1 + exp((-*sample * q)))) - ((-INT16_MIN + INT16_MAX) / 2);

    *sample = temporaryInt32;
}
