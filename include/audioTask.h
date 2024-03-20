#include <Sample.h>

#include <math.h>

TaskHandle_t audioTaskHandle;

void audioTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
        // Write all playing samples buffers to _masterBuffer
        for (int sampleFileRefIndex = 0; sampleFileRefIndex < statePointer->samples.size(); sampleFileRefIndex++)
        {
            Sample *sample = &statePointer->samples[sampleFileRefIndex];
            if (sample->isPlaying)
            {

                int fileSizeInSamples = sample->fileSize / sizeof(int16_t);

                // float playbackSpeed = 1.0;
                // if (sample->pitch >= 0)
                // {
                //     playbackSpeed = 1.0 + ((float)sample->pitch / 12);
                // }
                // else
                // {
                //     playbackSpeed = 1.0 + (1 / 2 / ((float)sample->pitch / 12));
                // }

                int sizeToWriteInSamples = 0;
                // if ((fileSizeInSamples - sample->bufferSamplesReadCounter) * playbackSpeed < PLAY_WAV_WAV_BUFFER_SIZE)
                if (fileSizeInSamples - sample->bufferSamplesReadCounter < PLAY_WAV_WAV_BUFFER_SIZE)
                {
                    // sizeToWriteInSamples = (fileSizeInSamples - sample->bufferSamplesReadCounter) / playbackSpeed;
                    sizeToWriteInSamples = fileSizeInSamples - sample->bufferSamplesReadCounter;
                }
                else
                {
                    sizeToWriteInSamples = PLAY_WAV_WAV_BUFFER_SIZE;
                }

                if (sizeToWriteInSamples <= 0)
                {
                    // printf("stop : %s\n", sample->filePath);
                    sample->isPlaying = false;
                    sample->bufferSamplesReadCounter = 0;
                    continue; // End of file or error
                }

                // Write sample buffer to _masterBuffer
                for (int i = 0; i < sizeToWriteInSamples; i++)
                {
                    // statePointer->_masterBuffer[i] += sample->buffer[(int)(sample->bufferSamplesReadCounter + roundf(i * playbackSpeed))] * sample->volume;
                    statePointer->_masterBuffer[i] += sample->buffer[sample->bufferSamplesReadCounter + i] * sample->volume;
                }

                // sample->bufferSamplesReadCounter += round(sizeToWriteInSamples * playbackSpeed);
                sample->bufferSamplesReadCounter += sizeToWriteInSamples;
            }
        }

        // Write _masterBuffer to I2S()
        size_t bytes_written; // Initialize bytes_written variable
        i2s_write(I2S_NUM_0, statePointer->_masterBuffer, PLAY_WAV_WAV_BUFFER_SIZE * sizeof(int16_t), &bytes_written, 1);
        for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
        {
            statePointer->_masterBuffer[i] = 0;
        }
    }
    vTaskDelete(audioTaskHandle);
}
