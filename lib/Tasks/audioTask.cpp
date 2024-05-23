#include <stdio.h>
#include <cstring>

#include <Tasks.h>
#include <math.h>

#include <Defs.h>
#include <Effects.h>
#include <MyUtils.h>

#include <driver/i2s.h>
// #include <driver/i2s_common.h>
// #include <driver/i2s_std.h>
// #include <driver/i2s_pdm.h>
// #include <driver/i2s_tdm.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
// // TODO : should work since c++17 flag is set
// #include "../../../../.platformio/packages/toolchain-xtensa-esp-elf/xtensa-esp-elf/include/c++/13.2.0/bits/algorithmfwd.h"

// TaskHandle_t audioTaskHandle;

void audioTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
        // Write all playing samples buffers to _masterBuffer
        for (int instrumentIndex = 0; instrumentIndex < statePointer->instruments.size(); instrumentIndex++)
        {
            // Instrument *instrument = &statePointer->instruments[instrumentIndex];
            if (statePointer->instruments[instrumentIndex].isPlaying)
            {
                bool isReverse = statePointer->instruments[instrumentIndex].isReverse xor statePointer->instruments[instrumentIndex].startingStepIsReverse;
                int fileSizeInSamples = statePointer->instruments[instrumentIndex].sample.fileSize / sizeof(int16_t);

                float playbackVolume = statePointer->instruments[instrumentIndex].volume * statePointer->instruments[instrumentIndex].startingStepVolume;
                float playbackSpeed = pitchToPlaybackSpeed(statePointer->instruments[instrumentIndex].pitch + statePointer->instruments[instrumentIndex].startingStepPitch);
                int playbackStartPositionInSample = round(statePointer->instruments[instrumentIndex].sample.fileSize / sizeof(int16_t) * ((statePointer->instruments[instrumentIndex].startingStepStartPosition == 0.0) ? statePointer->instruments[instrumentIndex].startPosition : statePointer->instruments[instrumentIndex].startingStepStartPosition));
                int playbackEndPositionInSample = round(statePointer->instruments[instrumentIndex].sample.fileSize / sizeof(int16_t) * ((statePointer->instruments[instrumentIndex].startingStepEndPosition == 1.0) ? statePointer->instruments[instrumentIndex].endPosition : statePointer->instruments[instrumentIndex].startingStepEndPosition));
                int restToReadFromFileSizeInSamples = playbackEndPositionInSample - playbackStartPositionInSample;

                int sizeToWriteInSamples = 0;
                int sizeIWantToWriteInSamples;
                if (isReverse)
                {
                    sizeIWantToWriteInSamples = round((restToReadFromFileSizeInSamples - (restToReadFromFileSizeInSamples - statePointer->instruments[instrumentIndex].bufferSamplesReadCounter)) / playbackSpeed);
                }
                else
                {
                    sizeIWantToWriteInSamples = round((restToReadFromFileSizeInSamples - statePointer->instruments[instrumentIndex].bufferSamplesReadCounter) / playbackSpeed);
                }

                if (sizeIWantToWriteInSamples < PLAY_WAV_WAV_BUFFER_SIZE)
                {
                    sizeToWriteInSamples = sizeIWantToWriteInSamples;
                }
                else
                {
                    sizeToWriteInSamples = PLAY_WAV_WAV_BUFFER_SIZE;
                }

                // When reaching sample end
                if (sizeToWriteInSamples <= 0)
                {
                    // printf("stop : %s\n", statePointer->instruments[instrumentIndex].sample.filePath);
                    statePointer->instruments[instrumentIndex].isPlaying = false;
                    statePointer->instruments[instrumentIndex].bufferSamplesReadCounter = 0;
                    continue; // End of file or error
                }

                if (isReverse)
                {
                    // Write sample buffer to _masterBuffer
                    for (int i = 0; i < sizeToWriteInSamples; i++)
                    {
                        // statePointer->_masterBuffer[i] += statePointer->instruments[instrumentIndex].buffer[(int)(statePointer->instruments[instrumentIndex].bufferSamplesReadCounter - round(i * playbackSpeed))] * statePointer->instruments[instrumentIndex].volume;
                        // TODO : Rethink this later ... It feels wrong
                        // TODO NB : temporaryInt32 outside the for loop ?
                        int32_t temporaryInt32 = 0;
                        temporaryInt32 += statePointer->_masterBuffer[i];
                        temporaryInt32 += statePointer->instruments[instrumentIndex].buffer[(int)(statePointer->instruments[instrumentIndex].bufferSamplesReadCounter - round(i * playbackSpeed))] * statePointer->instruments[instrumentIndex].volume;
                        temporaryInt32 = clip(temporaryInt32, (int32_t)INT16_MIN, (int32_t)INT16_MAX);
                        statePointer->_masterBuffer[i] = temporaryInt32;
                        // masterEffectCompressor(&statePointer->_masterBuffer[i]);
                        // masterEffectDistortion(&statePointer->_masterBuffer[i]);
                    }
                    statePointer->instruments[instrumentIndex].bufferSamplesReadCounter -= round(sizeToWriteInSamples * playbackSpeed);
                }
                else
                {
                    // Write sample buffer to _masterBuffer
                    for (int i = 0; i < sizeToWriteInSamples; i++)
                    {
                        // statePointer->_masterBuffer[i] += statePointer->instruments[instrumentIndex].buffer[(int)(statePointer->instruments[instrumentIndex].bufferSamplesReadCounter + round(i * playbackSpeed))] * statePointer->instruments[instrumentIndex].volume;
                        // TODO : Rethink this later ... It feels wrong
                        // TODO NB : temporaryInt32 outside the for loop ?
                        int32_t temporaryInt32 = 0;
                        temporaryInt32 += statePointer->_masterBuffer[i];
                        temporaryInt32 += statePointer->instruments[instrumentIndex].buffer[(int)(statePointer->instruments[instrumentIndex].bufferSamplesReadCounter + round(i * playbackSpeed))] * statePointer->instruments[instrumentIndex].volume;
                        temporaryInt32 = clip(temporaryInt32, (int32_t)INT16_MIN, (int32_t)INT16_MAX);
                        statePointer->_masterBuffer[i] = temporaryInt32;
                    }

                    statePointer->instruments[instrumentIndex].bufferSamplesReadCounter += round(sizeToWriteInSamples * playbackSpeed);
                }
            }
        }

        for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
        {
            masterEffectPreamp(statePointer, &statePointer->_masterBuffer[i]);
            if (statePointer->isBlbl)
            {
                // masterEffectCompressor(&statePointer->_masterBuffer[i]);
                masterEffectDistortion(&statePointer->_masterBuffer[i]);
            }
        }
        // if (statePointer->isBlbl)
        // // if (true)
        // {
        //     // TODO : WTF ** ???
        //     // masterEffectHallReverb((int16_t *)&statePointer->_masterBuffer);
        //     masterEffectReverb((int16_t *)&statePointer->_masterBuffer);
        // } else {
        //     // masterEffectDelay((int16_t *)&statePointer->_masterBuffer);
        // }

        // Write _masterBuffer to I2S()
        size_t bytes_written; // Initialize bytes_written variable

        i2s_write(I2S_NUM_0, statePointer->_masterBuffer, sizeof(statePointer->_masterBuffer), &bytes_written, 1);
        // i2s_channel_write(statePointer->tx_handle, buffer, bytes_read, &bytes_written, portMAX_DELAY);

        memset(statePointer->_masterBuffer, 0, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
    }
    // TODO : WHAT THE FUCK
    // vTaskDelete(audioTaskHandle);
}
