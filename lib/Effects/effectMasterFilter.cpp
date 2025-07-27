// #include <Defs.h>
#include <Effects.h>

void masterEffectFilter(int16_t *sample)
{
    float threshold = 0.5;
    // float ratio = 1 / 2;
    int ratio = 2;
    float q = 0.5;
    int attackInMs = 20;
    int releaseInMs = 200;
    
    float dryWetRatio = 0.75;
    float inputGain = 1.0;
    float outputGain = 1.0;

    
}
