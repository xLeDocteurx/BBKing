#include <Sample.h>

TaskHandle_t audioTaskHandle;

void audioTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
        // Write all playing samples buffers to _masterBuffer
        // statePointer->global.songs[statePointer->local.currentSongIndex].parts[statePointer->local.currentPartIndex].drumRack.writeToOutputBuffer();
        for (int sampleFileRefIndex = 0; sampleFileRefIndex < statePointer->samples.size(); sampleFileRefIndex++)
        {
            Sample *sample = &statePointer->samples[sampleFileRefIndex];
            if (sample->isPlaying)
            // if (sampleFileRefIndex == 6)
            {
                // // const FILE *fileRefPointer = sample->file;
                // const int fseekReturn = fseek(sample->file, 44 + sample->bufferBytesReadCounter, SEEK_SET); // Skip WAV file header (44 bytes) + bufferBytesReadCounter

                // int16_t standardSizeBuffer[PLAY_WAV_WAV_BUFFER_SIZE];

                // // size_t bytes_read = fread(warpedBuffer, sizeof(int16_t) / 2, sizeToRead, this->samples[i].file);
                // size_t bytes_read = fread(standardSizeBuffer, sizeof(int16_t) / 2, PLAY_WAV_WAV_BUFFER_SIZE, sample->file);

                // if (fseekReturn < 0 || bytes_read <= 0)
                // {
                //     // printf("stop : %s\n", statePointer->samples[sampleFileRefIndex].filePath);
                //     // fclose(file);
                //     sample->isPlaying = false;
                //     sample->bufferBytesReadCounter = 0;
                //     continue; // End of file or error
                // }
                int fileSizeInSamples = sample->fileSize / 2;
                int sizeToWrite = 0;
                if (fileSizeInSamples - sample->bufferBytesReadCounter < PLAY_WAV_WAV_BUFFER_SIZE)
                {
                    sizeToWrite = fileSizeInSamples - sample->bufferBytesReadCounter;
                }
                else
                {
                    sizeToWrite = PLAY_WAV_WAV_BUFFER_SIZE;
                }

                if (sizeToWrite <= 0)
                {
                    // printf("stop sample : %s\n", sample->filePath);
                    sample->isPlaying = false;
                    sample->bufferBytesReadCounter = 0;
                    continue; // End of file or error
                }

                // Write sample buffer to _masterBuffer
                for (int i = 0; i < sizeToWrite; i++)
                {
                    // statePointer->_masterBuffer[i] += standardSizeBuffer[i] * 0.5;
                    statePointer->_masterBuffer[i] += sample->buffer[sample->bufferBytesReadCounter + i] * 0.5;
                }

                // sample->bufferBytesReadCounter += sizeToRead;
                // sample->bufferBytesReadCounter += bytes_read;
                sample->bufferBytesReadCounter += sizeToWrite;
            }
        }

        // Write _masterBuffer to I2S()
        size_t bytes_written; // Initialize bytes_written variable
        i2s_write(I2S_NUM_0, statePointer->_masterBuffer, PLAY_WAV_WAV_BUFFER_SIZE, &bytes_written, 1);
        for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
        {
            statePointer->_masterBuffer[i] = 0;
        }
    }
    vTaskDelete(audioTaskHandle);
}
