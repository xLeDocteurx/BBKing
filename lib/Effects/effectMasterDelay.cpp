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

// bool ismasterDelayMemoryBufferInit = false;
// const size_t masterDelayMemoryBufferLength = 2 * DMA_WAV_SAMPLE_RATE;
// int16_t masterDelayMemoryBuffer[masterDelayMemoryBufferLength] = {};
// int16_t currentFrameBuffer[PLAY_WAV_WAV_BUFFER_SIZE] = {};
// int16_t previousFrameBuffer[PLAY_WAV_WAV_BUFFER_SIZE] = {};

const int masterDelayDelayTimeInMS = 150;
const int masterDelayDelayTimeInSamples = ((float)masterDelayDelayTimeInMS / 1000) * DMA_WAV_SAMPLE_RATE;
// std::vector<int16_t> masterDelayMemoryBufferAsVector(masterDelayDelayTimeInSamples, 0);
int16_t masterDelayMemoryBuffer[masterDelayDelayTimeInSamples] = {0};
float masterDelayFeedback = 0.35;
void masterEffectDelay(int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE])
{
    int16_t masterDelayMemoryBufferChunk[PLAY_WAV_WAV_BUFFER_SIZE];

    memcpy(masterDelayMemoryBufferChunk, masterDelayMemoryBuffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
    int startIndex = 0;
    int endIndex = PLAY_WAV_WAV_BUFFER_SIZE - 1;
    int numOfElements = masterDelayDelayTimeInSamples - (endIndex - startIndex + 1);
    memmove(masterDelayMemoryBuffer + startIndex, masterDelayMemoryBuffer + endIndex + 1, numOfElements * sizeof(int16_t));
    int arraysDiff = masterDelayDelayTimeInSamples - PLAY_WAV_WAV_BUFFER_SIZE;
    for (size_t i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
    {
        buffer[i] = (int16_t)(buffer[i] + ((float)masterDelayMemoryBufferChunk[i] * masterDelayFeedback));
    }
    memmove(masterDelayMemoryBuffer + arraysDiff, buffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
    memcpy(buffer, masterDelayMemoryBufferChunk, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
}
