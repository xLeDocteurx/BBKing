#include <Defs.h>
#include <Effects.h>

void masterEffectDistortion(int16_t *sample)
{
    // float threshold = 0.5;
    float q = 0.5;

    float dryWetRatio = 1.0;
    float inputGain = 1.0;
    float outputGain = 1.0;

    // int32_t min = INT16_MIN;
    // int32_t max = INT16_MAX;
    int32_t x = (*sample + INT16_MIN) / 2;
    // int32_t res = x * x * (3.0 - 3.0 * x) * (-INT16_MIN + INT16_MAX) - (-INT16_MIN);
    int32_t res = x * x * x * (x * 6 - 15) * (-INT16_MIN + INT16_MAX) - (-INT16_MIN);
    *sample = res;
}
