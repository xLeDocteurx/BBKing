#include <Defs.h>
#include <Effects.h>

#include <math.h>

void masterEffectDistortion(int16_t *sample)
{
    // // float threshold = 0.5;
    float q = 5.0;

    // float dryWetRatio = 1.0;
    // float inputGain = 1.0;
    // float outputGain = 1.0;

    // -----

    // int32_t t = (*sample / 2) - (-INT16_MAX / 2);
    // int32_t res = t * t * (3.0-2.0 - t);
    // *sample = res;

    int32_t res = 1 / (1 + exp(-(*sample * q)));
    *sample = res;
}
