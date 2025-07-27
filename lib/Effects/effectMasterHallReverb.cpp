#include <Defs.h>
#include <Effects.h>
#include <MyUtils.h>

#include <vector>
#include <math.h>
#include <complex.h>
// #include <fftw3.h>
// #include <ESP_fft.h>
#include <cstdint>
#include <cstring>

#include "esp_dsp.h"

int16_t *hallImpulseFileBufferPointer = NULL;
int hallImpulseFileSizeInSamples = 0;
// std::vector<int16_t> memoryBufferAsVector(PLAY_WAV_WAV_BUFFER_SIZE + hallImpulseFileSizeInSamples - 1);
// std::vector<int16_t> memoryBufferAsVector(PLAY_WAV_WAV_BUFFER_SIZE + hallImpulseFileSizeInSamples);
// int16_t memoryBufferAsArray[PLAY_WAV_WAV_BUFFER_SIZE + hallImpulseFileSizeInSamples - 1];
// float memoryBufferAsArray[100000];
// float *memoryBufferAsArray;
// float memoryBufferAsArray[PLAY_WAV_WAV_BUFFER_SIZE + 77143];
// float *memoryBufferAsArray = NULL;
const size_t memoryBufferAsArrayLength = 11249 + PLAY_WAV_WAV_BUFFER_SIZE;
float convolutionFrameAsArray[memoryBufferAsArrayLength] = {};
float memoryBufferAsArray[memoryBufferAsArrayLength] = {};
// TODO : malloc !!!
float hallImpulseFileBufferAsFloats[memoryBufferAsArrayLength] = {};

void masterEffectHallReverb(int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE])
// void masterEffectHallReverb(int16_t **buffer, int16_t **outputBufferPointer)
{
    if (hallImpulseFileBufferPointer == NULL)
    {
        char *impulseFilePath = "/data/room.wav";
        FILE *file = fopen(impulseFilePath, "r");
        if (file == NULL)
        {
            printf("Failed to open file : %s\n", impulseFilePath);
            return;
        }
        // printf("1.1\n");
        WavHeader header;
        fread(reinterpret_cast<char *>(&header), sizeof(uint8_t), sizeof(WavHeader), file);
        // Check if the file is a WAV file
        if (std::string(header.chunkID, 4) != "RIFF" || std::string(header.format, 4) != "WAVE")
        {
            printf("Not a WAV file! %s\n", impulseFilePath);
            return;
        }
        fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)
        hallImpulseFileSizeInSamples = header.subchunk2Size / sizeof(int16_t);
        hallImpulseFileBufferPointer = (int16_t *)malloc(header.subchunk2Size);

        // printf("1.2\n");

        fread(hallImpulseFileBufferPointer, sizeof(int16_t), hallImpulseFileSizeInSamples, file);
        fclose(file);
        printf("hallImpulseFileSizeInSamples : %i\n", hallImpulseFileSizeInSamples);

        // TODO
        for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
        {
            hallImpulseFileBufferAsFloats[i] = (float)hallImpulseFileBufferPointer[i];
            // printf("hallImpulseFileBufferAsFloats[i] : %f\n", hallImpulseFileBufferAsFloats[i]);
        }

        // memoryBufferAsArray = (float *)malloc(PLAY_WAV_WAV_BUFFER_SIZE + hallImpulseFileSizeInSamples - 1);

        // printf("1.3\n");²

        // esp_err_t ret;
        // ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
        // if (ret != ESP_OK)
        // {
        //     printf("Not possible to initialize FFT. Error = %i\n", ret);
        //     return;
        // }
        // printf("1.4\n");
        // for (size_t i = 0; i < hallImpulseFileSizeInSamples; i++)
        // {
        //     printf("hallImpulseFileSizeInSamples : %i\n", hallImpulseFileBufferPointer[i]);
        // }

        for (size_t i = 0; i < memoryBufferAsArrayLength; i++)
        {
            // printf("convolutionFrameAsArray[i] : %f\n", convolutionFrameAsArray[i]);
            memoryBufferAsArray[i] = 0;
        }
    }
    // printf("2\n");

    float bufferAsFloats[PLAY_WAV_WAV_BUFFER_SIZE] = {};
    for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
    {
        // printf("---");
        // printf("buffer[i] : %i\n", buffer[i]);
        bufferAsFloats[i] = (float)buffer[i];
        // printf("bufferAsFloats[i] : %f\n", bufferAsFloats[i]);
    }

    dsps_conv_f32_ae32(bufferAsFloats, PLAY_WAV_WAV_BUFFER_SIZE, hallImpulseFileBufferAsFloats, hallImpulseFileSizeInSamples / 16, convolutionFrameAsArray);

    // printf("4\n");

    // // TODO : Only loop to fill buffer ( do some search. I'm sure it can be prevented and I can copy directly a chunk of the memory buffer into the output buffer in a single operation )
    // // TODO : Handle the ERASE / FILL memorybufferVector outside the loop in one opération
    // for (size_t i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
    // {
    //     // // *outputBufferPointer[i] = memoryBufferAsVector.front();
    //     // *outputBufferPointer[i] = memoryBufferAsVector.front();
    //     // // *outputBufferPointer[i] = memoryBufferAsVector[i];
    //     // memoryBufferAsVector.erase(memoryBufferAsVector.begin());
    //     // memoryBufferAsVector.push_back(0);

    //     *outputBufferPointer[i] = memoryBufferAsArray[i];
    // }
    for (size_t i = 0; i < memoryBufferAsArrayLength; i++)
    {
        memoryBufferAsArray[i] += convolutionFrameAsArray[i];

        // // memcpy(*outputBufferPointer, memoryBufferAsArray, PLAY_WAV_WAV_BUFFER_SIZE);z
        // for (size_t i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
        // {
        //     // *outputBufferPointer[i] = memoryBufferAsArray[i];
        //     *outputBufferPointer[i] = (int16_t)memoryBufferAsArray[i];
        //     // *outputBufferPointer[i] = buffer[i];
        // }
        // int startIndex = 0;
        // int endIndex = PLAY_WAV_WAV_BUFFER_SIZE - 1;
        // int numOfElements = endIndex - startIndex + 1;
        // memmove(memoryBufferAsArray + startIndex, memoryBufferAsArray + endIndex + 1, numOfElements * sizeof(int));
        // // memoryBufferAsVector.push_back(0);
        // // TODO : Rethink the "memoryBufferAsArrayLength - 1" !!!
        // for (size_t i = memoryBufferAsArrayLength - 1 - PLAY_WAV_WAV_BUFFER_SIZE; i < memoryBufferAsArrayLength; i++)
        // {
        //     memoryBufferAsArray[i] = 0;
        // }
        if (i < PLAY_WAV_WAV_BUFFER_SIZE)
        {

            // *outputBufferPointer[i] = memoryBufferAsArray[i];
            // outputBufferPointer[i] = (int16_t)clip(memoryBufferAsArray[i], (float)INT16_MIN, (float)INT16_MAX);

            buffer[i] = (int16_t)clip(memoryBufferAsArray[i], (float)INT16_MIN, (float)INT16_MAX);
            // outputBufferPointer[i] = (int16_t)clip((memoryBufferAsArray[i] * INT16_MAX) / __FLT_MAX__, (float)INT16_MIN, (float)INT16_MAX);
            // outputBufferPointer[i] = (memoryBufferAsArray[i] * INT16_MAX) / __FLT_MAX__;

            // outputBufferPointer[i] = buffer[i];
            // *outputBufferPointer[i] = rand() % (INT16_MAX - INT16_MIN + 1) + INT16_MIN;
        }
    }

    // printf("5\n");

    // // Libération des buffers
    // free(input_real);
    // free(input_imag);
    // free(impulse_real);
    // free(impulse_imag);
    // free(result_real);
    // free(result_imag);

    // printf("6\n");
}
