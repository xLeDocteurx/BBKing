#include <vector>
#include <string>
#include <stdio.h>
#include "driver/gpio.h"
// #include <esp_http_server.h>
// #include <esp_wifi.h>

// #include <State.h>

// #ifndef GLOBALVARS_H
// #define GLOBALVARS_H

#define LED_PIN GPIO_NUM_2
#define KEYBOARD_CLOCK_PIN GPIO_NUM_34
#define KEYBOARD_DATA_PIN GPIO_NUM_35
#define JSON_BUFFER_SIZE 512

#define DMA_WAV_SAMPLE_RATE 44100
#define DMA_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
// #define DMA_CHANNEL_FORMAT I2S_CHANNEL_FMT_ONLY_LEFT
// #define DMA_CHANNEL_FORMAT I2S_CHANNEL_FMT_ONLY_RIGHT
#define DMA_CHANNEL_FORMAT I2S_CHANNEL_FMT_RIGHT_LEFT
#define DMA_WAV_BUFFER_COUNT 8
// #define DMA_WAV_BUFFER_SIZE 128
#define DMA_WAV_BUFFER_SIZE 64
// #define DMA_WAV_BUFFER_SIZE 32
// #define PLAY_WAV_WAV_BUFFER_SIZE 8 * 128 // 1024
#define PLAY_WAV_WAV_BUFFER_SIZE 8 * 64
// #define PLAY_WAV_WAV_BUFFER_SIZE 8 * 32

// For lolin32
// #define I2S_BCK_IO_NUM 26
// #define I2S_WS_IO_NUM 25
// #define I2S_DATA_OUT_NUM 22

// For freenove_esp32_s3_wroom
#define I2S_BCK_IO_NUM 46
#define I2S_WS_IO_NUM 47
#define I2S_DATA_OUT_NUM 45

#define STATE_PART_STEPS_LENGTH 16

// #endif // GLOBALVARS_H



#ifndef DEFS_H
#define DEFS_H

struct Step
{
    int instrumentIndex;
    int pitch;
    float volume;
};

struct Part
{
    int staves;
    // std::vector<std::vector<int>> steps;
    std::vector<std::vector<Step>> steps;
};

struct Sample
{
    char *filePath;
    bool isMono;
    float volume;
    int pitch;

    size_t fileSize;
    int16_t *buffer;    
    bool isPlaying;
    int bufferSamplesReadCounter;
    int startingStepVolume;
    int startingStepPitch;
};

struct Instrument
{
    int sampleFileRefIndex;
};

struct State
{
    int currentSongIndex;
    int currentModeIndex;
    int currentSelectedStepIndex;
    char *songName;
    int songTempo;

    std::vector<Sample> samples;
    // drumRack
    std::vector<Instrument> instruments;
    // slicer
    // int slicerSampleFileRefIndex;
    // sampler
    // int samplerSampleFileRefIndex;
    // synth

    int currentPartIndex;
    // Instrument currentPartInstrument;
    int currentPartInstrumentIndex;
    int currentStaveIndex;
    int currentOctaveIndex;
    // parts
    std::vector<Part> parts;

    int currentStepIndex;

    // Effects
    float effectMasterDistortionGain;
    float effectMasterDistortionOutputGain;

    int16_t _masterBuffer[PLAY_WAV_WAV_BUFFER_SIZE];
    bool isPlaying;
};

struct WifiConfig
{
    std::string ssid;
    std::string password;
};

#endif // DEFS_H