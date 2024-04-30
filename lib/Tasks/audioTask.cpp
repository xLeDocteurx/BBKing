#include <stdio.h>
#include <cstring>

#include <Tasks.h>
#include <math.h>

#include <Defs.h>
#include <MyUtils.h>

#include <driver/i2s.h>
// #include <driver/i2s_common.h>
// #include <driver/i2s_std.h>
// #include <driver/i2s_pdm.h>
// #include <driver/i2s_tdm.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// TaskHandle_t audioTaskHandle;

void audioTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
        // Write all playing samples buffers to _masterBuffer
        for (int instrumentIndex = 0; instrumentIndex < statePointer->instruments.size(); instrumentIndex++)
        {
            Instrument *instrument = &statePointer->instruments[instrumentIndex];
            if (instrument->isPlaying)
            {
                int fileSizeInSamples = instrument->sample.fileSize / sizeof(int16_t);

                // TODO : En function utils "pitchToSpeed"
                // float playbackSpeed = pow(2, static_cast<float>(instrument->pitch + instrument->startingStepPitch) / 12.0);
                float playbackSpeed = pitchToPlaybackSpeed(instrument->pitch + instrument->startingStepPitch);

                int sizeToWriteInSamples = 0;
                int sizeIWantToWriteInSamples = (fileSizeInSamples - instrument->bufferSamplesReadCounter) / playbackSpeed;
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
                    // printf("stop : %s\n", sample->filePath);
                    instrument->isPlaying = false;
                    instrument->bufferSamplesReadCounter = 0;
                    continue; // End of file or error
                }

                // Write sample buffer to _masterBuffer
                for (int i = 0; i < sizeToWriteInSamples; i++)
                {
                    // statePointer->_masterBuffer[i] += sample->buffer[(int)(sample->bufferSamplesReadCounter + round(i * playbackSpeed))] * sample->volume * sample->startingStepVolume;
                    // printf("sample->startingStepVolume : %i\n", sample->startingStepVolume);
                    statePointer->_masterBuffer[i] += instrument->buffer[(int)(instrument->bufferSamplesReadCounter + round(i * playbackSpeed))] * instrument->volume;
                }

                instrument->bufferSamplesReadCounter += round(sizeToWriteInSamples * playbackSpeed);
            }
        }

        // MasterEffectsStage
        // masterEffectDistortion(statePointer)

        // Write _masterBuffer to I2S()
        size_t bytes_written; // Initialize bytes_written variable

        // i2s_write(I2S_NUM_0, statePointer->_masterBuffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t), &bytes_written, 1);
        i2s_write(I2S_NUM_0, statePointer->_masterBuffer, sizeof(statePointer->_masterBuffer), &bytes_written, 1);
        // i2s_channel_write(statePointer->tx_handle, buffer, bytes_read, &bytes_written, portMAX_DELAY);
        // i2s_channel_write(statePointer->tx_handle, statePointer->_masterBuffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t), &bytes_written, 1);
        // i2s_channel_write(statePointer->tx_handle, statePointer->_masterBuffer, sizeof(statePointer->_masterBuffer), &bytes_written, 1);

        memset(statePointer->_masterBuffer, 0, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
    }
    // TODO : WHAT THE FUCK
    // vTaskDelete(audioTaskHandle);
}
