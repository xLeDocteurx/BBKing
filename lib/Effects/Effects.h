#include <Defs.h>

#ifndef EFFECTS_H
#define EFFECTS_H

void masterEffectPreamp(State *statePointer, int16_t *sample);

void masterEffectDistortion(int16_t *sample);
void masterEffectCompressor(int16_t *sample);

#endif // EFFECTS_H
