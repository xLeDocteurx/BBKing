#include <Defs.h>

#ifndef EFFECTS_H
#define EFFECTS_H

void effectPreamp(float gain, int16_t *sample);

void masterEffectDelay(int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE]);
void masterEffectReverb(int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE]);

void masterEffectHallReverb(int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE]);

void masterEffectCompressor(int16_t *sample);
void masterEffectDistortion(int16_t *sample);
void masterEffectFilter(int16_t *sample);

#endif // EFFECTS_H
