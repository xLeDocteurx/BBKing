#include <Defs.h>

#ifndef EFFECTS_H
#define EFFECTS_H

void masterEffectPreamp(State *statePointer, int16_t *sample);

void masterEffectMineReverb(int16_t *inputBuffer[PLAY_WAV_WAV_BUFFER_SIZE], int16_t *outputBuffer[PLAY_WAV_WAV_BUFFER_SIZE]);
void masterEffectHallReverb(int16_t *inputBuffer[PLAY_WAV_WAV_BUFFER_SIZE], int16_t *outputBuffer[PLAY_WAV_WAV_BUFFER_SIZE]);
void masterEffectRoomReverb(int16_t *inputBuffer[PLAY_WAV_WAV_BUFFER_SIZE], int16_t *outputBuffer[PLAY_WAV_WAV_BUFFER_SIZE]);

void masterEffectCompressor(int16_t *sample);
void masterEffectDistortion(int16_t *sample);

#endif // EFFECTS_H
