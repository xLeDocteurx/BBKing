#include <Defs.h>
#include <Effects.h>

#include <vector>
#include <math.h>
#include <complex.h>
// #include <fftw3.h>
// #include <ESP_fft.h>

int16_t **hallImpulseFileBufferPointer = NULL;
int hallImpulseFileSizeInSamples = 0;
std::vector<int16_t> memoryBufferAsVector = {};

void masterEffectHallReverb(int16_t *inputBuffer[PLAY_WAV_WAV_BUFFER_SIZE], int16_t *outputBuffer[PLAY_WAV_WAV_BUFFER_SIZE])
{
    // float threshold = 0.5;
    // // float ratio = 1 / 2;
    // int ratio = 2;
    // float q = 0.5;
    // int attackInMs = 20;
    // int releaseInMs = 200;

    // float dryWetRatio = 0.75;
    // float inputGain = 1.0;
    // float outputGain = 1.0;

    if (hallImpulseFileBufferPointer == NULL)
    {
        char *impulseFilePath = "/data/hall.wav";
        FILE *file = fopen(impulseFilePath, "r");
        if (file == NULL)
        {
            printf("Failed to open file : %s\n", impulseFilePath);
            return;
        }
        WavHeader header;
        fread(reinterpret_cast<char *>(&header), sizeof(u_int8_t), sizeof(WavHeader), file);
        // Check if the file is a WAV file
        if (std::string(header.chunkID, 4) != "RIFF" || std::string(header.format, 4) != "WAVE")
        {
            printf("Not a WAV file! %s\n", impulseFilePath);
            return;
        }
        fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)
        hallImpulseFileSizeInSamples = header.subchunk2Size / sizeof(int16_t);
        hallImpulseFileBufferPointer = (int16_t *)malloc(header.subchunk2Size);

        fread(hallImpulseFileBufferPointer, sizeof(int16_t), hallImpulseFileSizeInSamples, file);
        fclose(file);
    }

    // void conv_fft(const int16_t *input1, const int16_t *input2, int16_t *output, int DMA_WAV_BUFFER_SIZE, int hallImpulseFileSizeInSamples) {
    // // Vérifier les longueurs des entrées
    // if (DMA_WAV_BUFFER_SIZE < hallImpulseFileSizeInSamples)
    // {
    //     return;
    // }

    // Déterminer la longueur de convolution requise
    int conv_len = DMA_WAV_BUFFER_SIZE + hallImpulseFileSizeInSamples - 1;

    // Allouer de la mémoire pour les données complexes
    std::complex<int16_t> *X1 = new std::complex<int16_t>[conv_len];
    std::complex<int16_t> *X2 = new std::complex<int16_t>[conv_len];
    std::complex<int16_t> *Y = new std::complex<int16_t>[conv_len];

    // Copier les données d'entrée dans les données complexes
    for (int i = 0; i < DMA_WAV_BUFFER_SIZE; i++)
    {
        X1[i] = std::complex<int16_t>(*inputBuffer[i], 0);
    }
    for (int i = 0; i < hallImpulseFileSizeInSamples; i++)
    {
        X2[i] = std::complex<int16_t>(*hallImpulseFileBufferPointer[i], 0);
    }

    // // Effectuer la transformée de Fourier rapide (FFT)
    // fftw3_plan inputPlan = fftw3_plan_dft_1d(conv_len, X1, X1, FFTW_FORWARD, FFTW_ESTIMATE);
    // fftw3_execute(inputPlan);
    // fftw3_destroy_plan(inputPlan);

    // fftw3_plan impulsePlan = fftw3_plan_dft_1d(conv_len, X2, X2, FFTW_FORWARD, FFTW_ESTIMATE);
    // fftw3_execute(impulsePlan);
    // fftw3_destroy_plan(impulsePlan);

    // // Effectuer la multiplication point par point
    // for (int i = 0; i < conv_len; i++)
    // {
    //     Y[i] = X1[i] * X2[i];
    // }

    // // Effectuer la transformée de Fourier rapide inverse (IFFT)
    // fftw3_plan plan3 = fftw3_plan_dft_1d(conv_len, Y, Y, FFTW_BACKWARD, FFTW_ESTIMATE);
    // fftw3_execute(plan3);
    // fftw3_destroy_plan(plan3);

    // // Normaliser la sortie
    // for (int i = 0; i < conv_len; i++)
    // {
    //     outputBuffer[i] = std::real(Y[i]) / conv_len;
    // }

    // // Libérer la mémoire allouée
    // delete[] X1;
    // delete[] X2;
    // delete[] Y;
}
