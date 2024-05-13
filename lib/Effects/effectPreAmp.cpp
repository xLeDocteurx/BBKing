#include <Defs.h>
#include <Effects.h>

void masterEffectPreamp(int16_t *sample)
{
    *sample = *sample * 2;
}
