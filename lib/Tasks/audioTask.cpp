#include <stdio.h>
#include <cstring>

#include <Tasks.h>
#include <math.h>

#include <Defs.h>

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

                // printf("instrumentIndex : %i\n", instrumentIndex);

                int fileSizeInSamples = instrument->sample.fileSize / sizeof(int16_t);

                // Step playbackSpeed and pitch

                // int stepPitch = sample->startingStepPitch;
                // float stepVolume = sample->startingStepVolume;
                // int stepPitch = 0;
                // float stepVolume = 0;

                // // printf("statePointer->currentStepIndex : %i\n", statePointer->currentStepIndex);
                // // printf("statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size() : %i\n", statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size());
                // for (int stepElementIndex = 0; stepElementIndex < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); stepElementIndex++)
                // {
                //     // printf("stepElementIndex : %i\n", stepElementIndex);
                //     int foundInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepElementIndex].instrumentIndex;
                //     // printf("foundInstrumentIndex : %i\n", stepElementIndex);
                //     int foundSampleIndex = getInstrumentSampleIndex(statePointer, foundInstrumentIndex);
                //     // printf("instrumentIndex : %i, foundSampleIndex : %i\n", instrumentIndex, foundSampleIndex);
                //     if (instrumentIndex == foundSampleIndex)
                //     {
                //         stepPitch = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepElementIndex].pitch;
                //         stepVolume = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepElementIndex].volume;
                //         if (instrumentIndex == 4)
                //         {
                // printf("step pitch %i\n", stepPitch);
                //         }
                //     }

                //     // TODO : Check alternate solution via find
                //     // std::vector<Step>::const_iterator it = std::find(statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].begin(), statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].end(), i);
                //     // if (it. != NULL)
                //     // {
                //     //     stepPitch = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].pitch;
                //     //     stepVolume = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].volume;
                //     // }
                // }

                // TODO : En function utils "pitchToSpeed"
                float playbackSpeed = pow(2, static_cast<float>(instrument->pitch + instrument->startingStepPitch) / 12.0);
                // if (instrumentIndex == 4)
                // {
                //     printf("playbackSpeed %f\n", playbackSpeed);
                // }

                // if (instrument->sample.isMono)
                // {
                //     playbackSpeed *= 0.5;
                // }

                int sizeToWriteInSamples = 0;
                int sizeIWantToWriteInSamples = (fileSizeInSamples - instrument->bufferSamplesReadCounter) / playbackSpeed;
                if (sizeIWantToWriteInSamples < PLAY_WAV_WAV_BUFFER_SIZE)
                {
                    sizeToWriteInSamples = sizeIWantToWriteInSamples;
                    // sizeToWriteInSamples = fileSizeInSamples - sample->bufferSamplesReadCounter;
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
                // else if (bytes_read < PLAY_WAV_WAV_BUFFER_SIZE)
                // {
                //     printf("next stop: /data/clap.wav : %i\n", PLAY_WAV_WAV_BUFFER_SIZE - bytes_read);
                //     continue;
                // }

                // Write sample buffer to _masterBuffer
                for (int i = 0; i < sizeToWriteInSamples; i++)
                {
                    // statePointer->_masterBuffer[i] += sample->buffer[(int)(sample->bufferSamplesReadCounter + round(i * playbackSpeed))] * sample->volume * sample->startingStepVolume;
                    // printf("sample->startingStepVolume : %i\n", sample->startingStepVolume);
                    statePointer->_masterBuffer[i] += instrument->buffer[(int)(instrument->bufferSamplesReadCounter + round(i * playbackSpeed))] * instrument->volume;
                }
                // TODO : If sizeToWriteInSamples is smaller than _masterBuffer size. Fill the rest with 0s.
                // for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE - sizeToWriteInSamples; i++)
                // {
                //     statePointer->_masterBuffer[i] += 0;
                // }

                instrument->bufferSamplesReadCounter += round(sizeToWriteInSamples * playbackSpeed);
            }
        }

        // MasterEffectsStage
        // masterEffectDistortion(statePointer)

        // Write _masterBuffer to I2S()
        size_t bytes_written; // Initialize bytes_written variable

        // TODO : Uncomment
        // i2s_write(I2S_NUM_0, statePointer->_masterBuffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t), &bytes_written, 1);
        i2s_write(I2S_NUM_0, statePointer->_masterBuffer, sizeof(statePointer->_masterBuffer), &bytes_written, 1);
        // i2s_channel_write(statePointer->tx_handle, buffer, bytes_read, &bytes_written, portMAX_DELAY);
        // i2s_channel_write(statePointer->tx_handle, statePointer->_masterBuffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t), &bytes_written, 1);
        // i2s_channel_write(statePointer->tx_handle, statePointer->_masterBuffer, sizeof(statePointer->_masterBuffer), &bytes_written, 1);

        // for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
        // {
        //     statePointer->_masterBuffer[i] = 0;
        // }
        memset(statePointer->_masterBuffer, 0, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t));
    }
    // TODO : WHAT THE FUCK
    // vTaskDelete(audioTaskHandle);
}
