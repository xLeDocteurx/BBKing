#include <variant>
#include <vector>
#include <memory>
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
// #define DMA_WAV_BUFFER_SIZE 256 // 2048
// #define DMA_WAV_BUFFER_SIZE 128 // 1024
#define DMA_WAV_BUFFER_SIZE 64 // 512
// #define DMA_WAV_BUFFER_SIZE 32 // 256
// #define PLAY_WAV_WAV_BUFFER_SIZE 8 * 256 // 2048
// #define PLAY_WAV_WAV_BUFFER_SIZE 8 * 128 // 1024
#define PLAY_WAV_WAV_BUFFER_SIZE 8 * 64 // 512
// #define PLAY_WAV_WAV_BUFFER_SIZE 8 * 32 // 256

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

enum InstrumentType
{
    DRUM_RACK = 0,
    SAMPLER = 1,
    SYNTH = 2
};

enum WaveFormType
{
    SINE = 0,
    TRIANGLE = 1,
    SAW = 2,
    SQUARE = 3
};

struct Instrument
{
    InstrumentType type;
    bool isSolo;
    bool isMuted;
    bool isPlaying;

    float volume;
    int previousStepVolume;
    int pitch;
    int previousStepPitch;

    int16_t *buffer;
    int bufferSamplesReadCounter;
};

struct DrumRack : public Instrument
{
    // int sampleFileRefIndex;
    Sample sample;

    bool isReverse;
    bool previousStepIsReverse;

    // TODO : In number of samples ???
    // For now heads are set up in relative units
    float startPosition;
    float previousStepStartPosition;
    float endPosition;
    float previousStepEndPosition;
};

struct Sampler : public Instrument
{
    // int sampleFileRefIndex;
    Sample sample;

    bool isLooping;

    // TODO : In number of samples ???
    // For now heads are set up in relative units
    float loopStartPosition;
    float loopEndPosition;

    float attackPosition;
    float previousStepAttackPosition;
    float decayPosition;
    float previousStepDecayPosition;
    float sustainPosition;
    float previousStepSustainPosition;
    float releasePosition;
    float previousStepReleasePosition;
};

struct Synth : public Instrument
{
    WaveFormType osc1WaveFormType;
    WaveFormType osc2WaveFormType;
    // TODO : Really need a 3rd one ?
    // Check Monark design
    WaveFormType osc3WaveFormType;
};

enum StepType
{
    DRUM_RACK_STEP = 0,
    SAMPLER_STEP = 1,
    SYNTH_STEP = 2,
};

struct Step
{
    // StepType type;

    int instrumentIndex;
    float volume;
    int pitch;
};

struct DrumRackStep : public Step
{
    bool isReverse;

    float startPosition;
    float endPosition;
};

struct SamplerStep : public Step
{
    // bool isLooping;

    // float loopStartPosition;
    // float loopEndPosition;

    // float attackPosition;
    // float decayPosition;
    // float sustainPosition;
    // float releasePosition;
};

struct SynthStep : public Step
{
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

enum ModeType
{
    PART = 0,
    STEP = 1,
    FX = 2,
    SAMPLE = 3
};

struct State
{
    // TODO : Remove
    bool isBlbl;

    // std::vector<std::string> wavFilePaths;
    std::vector<char *> wavFilePaths;

    float masterGain;

    int currentSongIndex;
    ModeType currentMode;
    int currentSelectedStepIndex;
    char *songName;
    int songTempo;

    // std::vector<Sample> samples;
    // drumRack
    // TODO : Move instruments into parts
    // std::vector<std::variant<DrumRack, Sampler, Synth>> instruments;
    // std::vector<DrumRack> instruments;
    std::vector<std::unique_ptr<Instrument>> instruments;
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