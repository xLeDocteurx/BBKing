#include <Defs.h>
#include <Effects.h>
#include <MyUtils.h>

// #include <vector>
// #include <math.h>
// #include <complex.h>
#include <cstdint>
#include <cstring>

// #include "esp_dsp.h"

// TODO : PSRAM malloc

// bool ismasterReverbMemoryBufferInit = false;
// const size_t masterReverbMemoryBufferLength = 2 * DMA_WAV_SAMPLE_RATE;
// int16_t masterReverbMemoryBuffer[masterReverbMemoryBufferLength] = {};
// int16_t currentFrameBuffer[PLAY_WAV_WAV_BUFFER_SIZE] = {};
// int16_t previousFrameBuffer[PLAY_WAV_WAV_BUFFER_SIZE] = {};

const int masterReverbDelayTimeInMS = 200;
const int masterReverbDelayTimeInSamples = ((float)masterReverbDelayTimeInMS / 1000) * DMA_WAV_SAMPLE_RATE;
// std::vector<int16_t> masterReverbMemoryBufferAsVector(masterReverbDelayTimeInSamples, 0);
int16_t masterReverbMemoryBuffer[masterReverbDelayTimeInSamples] = {0};
float masterReverbFeedback = 0.3;

// TODO : outside function
int arraysDiff = masterReverbDelayTimeInSamples - PLAY_WAV_WAV_BUFFER_SIZE;
// TODO : outside function
int ph2 = masterReverbDelayTimeInSamples / 2;
int ph3 = masterReverbDelayTimeInSamples / 3;
int ph4 = masterReverbDelayTimeInSamples / 4;
int ph5 = masterReverbDelayTimeInSamples / 5;
void masterEffectReverb(int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE])
{
    int16_t masterReverbMemoryBufferChunk[PLAY_WAV_WAV_BUFFER_SIZE];

    memcpy(masterReverbMemoryBufferChunk, masterReverbMemoryBuffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
    int startIndex = 0;
    int endIndex = PLAY_WAV_WAV_BUFFER_SIZE - 1;
    int numOfElements = masterReverbDelayTimeInSamples - (endIndex - startIndex + 1);
    memmove(masterReverbMemoryBuffer + startIndex, masterReverbMemoryBuffer + endIndex + 1, numOfElements * sizeof(int16_t));
    for (size_t i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
    {
        int16_t newValue = (int16_t)(buffer[i] + ((float)masterReverbMemoryBufferChunk[i] * masterReverbFeedback));
        buffer[i] = newValue;
        // masterReverbMemoryBuffer[ph2 + i] += newValue;
        masterReverbMemoryBuffer[ph3 + i] += newValue;
        // masterReverbMemoryBuffer[ph4 + i] += newValue / 2;
        // masterReverbMemoryBuffer[ph5 + i] += newValue / 2;
        // masterReverbMemoryBuffer[ph2 + ph5 + i] += newValue / 2;
        // masterReverbMemoryBuffer[ph3 + ph5 + i] += newValue / 2;
        // masterReverbMemoryBuffer[ph4 + ph5 + i] += newValue / 2;
    }
    memmove(masterReverbMemoryBuffer + arraysDiff, buffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));

    // for (size_t i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
    // {
    //     masterReverbMemoryBuffer[ph1 + i] = (int16_t)(masterReverbMemoryBuffer[ph1 + i] + ((float)masterReverbMemoryBufferChunk[i] * masterReverbFeedback));
    //     masterReverbMemoryBuffer[ph3 + i] = (int16_t)(masterReverbMemoryBuffer[ph1 + i] + ((float)masterReverbMemoryBufferChunk[i] * masterReverbFeedback));
    // }

    memcpy(buffer, masterReverbMemoryBufferChunk, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
}
