#include <vector>
#include <string>
#include <stdio.h>
#include "driver/gpio.h"
// #include <esp_http_server.h>
// #include <esp_wifi.h>
// #include <driver/i2s.h>
#include <driver/i2s_common.h>
#include <driver/i2s_std.h>
#include <driver/i2s_pdm.h>
#include <driver/i2s_tdm.h>

// #include <State.h>

// #ifndef GLOBALVARS_H
// #define GLOBALVARS_H

#define LED_PIN GPIO_NUM_2
#define KEYBOARD_CLOCK_PIN GPIO_NUM_34
#define KEYBOARD_DATA_PIN GPIO_NUM_35
#define JSON_BUFFER_SIZE 512

#define DMA_WAV_SAMPLE_RATE 44100
#define DMA_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define DMA_CHANNEL_FORMAT I2S_CHANNEL_FMT_ONLY_LEFT
// #define DMA_CHANNEL_FORMAT I2S_CHANNEL_FMT_ONLY_RIGHT
// #define DMA_CHANNEL_FORMAT I2S_CHANNEL_FMT_RIGHT_LEFT
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

// #define PIN_NUM_MISO GPIO_NUM
// #define PIN_NUM_MOSI GPIO_NUM
// #define PIN_NUM_CLK GPIO_NUM
// #define PIN_NUM_CS GPIO_NUM
#define SD_MMC_CMD GPIO_NUM_38 // Please do not modify it.
#define SD_MMC_CLK GPIO_NUM_39 // Please do not modify it.
#define SD_MMC_D0 GPIO_NUM_40  // Please do not modify it.

#define I2S_BCK_IO_NUM GPIO_NUM_46
#define I2S_WS_IO_NUM GPIO_NUM_47
#define I2S_DATA_OUT_NUM GPIO_NUM_45

#define STATE_PART_STEPS_LENGTH 16

// #endif // GLOBALVARS_H

#ifndef DEFS_H
#define DEFS_H

struct Sample
{
    // TODO : update as an index for wavFilePaths !?!
    const char *filePath;
    bool isMono;

    size_t fileSize;
};

struct Instrument
{
    // int sampleFileRefIndex;
    Sample sample;
    bool isSolo;
    bool isMuted;

    float volume;
    int pitch;
    // TODO : In number of samples ???
    // For now heads are set up in relative units
    float startPosition;
    float endPosition;
    bool isReverse;

    int16_t *buffer;
    bool isPlaying;
    int bufferSamplesReadCounter;
    int startingStepVolume;
    int startingStepPitch;
    float startingStepStartPosition;
    float startingStepEndPosition;
    bool startingStepIsReverse;
};
struct Step
{
    int instrumentIndex;
    float volume;
    int pitch;
    float startPosition;
    float endPosition;
    bool isReverse;
};

struct Part
{
    int staves;

    // TODO : Move instruments into parts
    // std::vector<Instrument> instruments;

    // std::vector<std::vector<int>> steps;
    std::vector<std::vector<Step>> steps;
};

// Define the WAV file header structure
struct WavHeader
{
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
};

struct State
{
    // std::vector<std::string> wavFilePaths;
    std::vector<char *> wavFilePaths;

    int currentSongIndex;
    int currentModeIndex;
    int currentSelectedStepIndex;
    char *songName;
    int songTempo;

    // std::vector<Sample> samples;
    // drumRack
    // TODO : Move instruments into parts
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

    i2s_chan_handle_t tx_handle;
    i2s_chan_handle_t rx_handle;
    int16_t _masterBuffer[PLAY_WAV_WAV_BUFFER_SIZE];
    bool isPlaying;
};

struct WifiConfig
{
    std::string ssid;
    std::string password;
};

#endif // DEFS_H