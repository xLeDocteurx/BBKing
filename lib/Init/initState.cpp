#include <vector>
#include <string>

#include <esp_psram.h>
#include <esp_system.h>
#include <driver/spi_master.h>

#include <Defs.h>
#include <MyUtils.h>
#include <Init.h>

// TODO : wtf ?
void freeFile(void *filePointer)
{
    free(filePointer);
}

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

// TODO : Merge loadSample and loadInstrument
bool loadSample(char *filePath, bool isMono, Sample *samplePointer)
{
    printf("loadSample(%s);\n", filePath);

    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", filePath);
        return false;
    }
    // fseek(file, 0, SEEK_END);

    // Read the WAV header
    WavHeader header;
    fread(reinterpret_cast<char *>(&header), sizeof(u_int8_t), sizeof(WavHeader), file);

    // Check if the file is a WAV file
    if (std::string(header.chunkID, 4) != "RIFF" || std::string(header.format, 4) != "WAVE")
    {
        printf("Not a WAV file! %s\n", filePath);
        return false;
    }

    // TODO : file size reading header data
    // size_t fileSize = ftell(file) - 44;
    size_t fileSize = header.subchunk2Size;
    printf("fileSize : %i\n", fileSize);
    // fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)

    //     int16_t *fileBufferPointer = (int16_t *)malloc(fileSize);
    // , fileBufferPointer, false, 0, 0, 0

    // size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t), fileSize / sizeof(int16_t), file);

    fclose(file);

    // TODO : isMono from header
    *samplePointer = {filePath, isMono, fileSize};
    return true;
}

bool loadInstrument(char *filePath, bool isMono, float volume, int pitch, Instrument *instrumentPointer)
{
    Sample sample;
    loadSample(filePath, isMono, &sample);

    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", filePath);
        return false;
    }
    fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)
    int16_t *fileBufferPointer = (int16_t *)malloc(sample.fileSize);

    // size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t), sample.fileSize / sizeof(int16_t), file);
    fread(fileBufferPointer, sizeof(int16_t), sample.fileSize / sizeof(int16_t), file);

    fclose(file);

    *instrumentPointer = {sample, false, false, volume, pitch, fileBufferPointer, false, 0, 0, 0};
    return true;
}

// bool initState(&statePointer auto)
bool initState(State *statePointer)
{
    // std::string samplesJsonString;
    // bool readJsonFile_ret = readJsonFile("/data/samples.json", &samplesJsonString);
    // if (!readJsonFile_ret)
    // {
    //     printf("Failed to read samples.json file\n");
    //     return false;
    // }
    // else
    // {
    //     // printf("initWifi readJsonFile songs.json : %s\n", samplesJsonString.c_str());
    // }
    // std::string songsJsonString;
    // readJsonFile_ret = readJsonFile("/data/songs.json", &songsJsonString);
    // if (!readJsonFile_ret)
    // {
    //     printf("Failed to read wifiConfigs.json file\n");
    //     return false;
    // }
    // else
    // {
    //     // printf("initWifi readJsonFile wifiConfigs.json : %s\n", songsJsonString.c_str());
    // }

    // Init state
    statePointer->currentModeIndex = 0;
    statePointer->currentSelectedStepIndex = 0;
    statePointer->currentSongIndex = 0;
    statePointer->songName = "Demo song";
    statePointer->songTempo = 154;

    // Instruments
    Instrument instrument1;
    loadInstrument("/data/kick.wav", true, 0.5, 0, &instrument1);
    statePointer->instruments.push_back(instrument1);
    Instrument instrument2;
    loadInstrument("/data/snare.wav", true, 0.5, 0, &instrument2);
    statePointer->instruments.push_back(instrument2);
    Instrument instrument3;
    loadInstrument("/data/clap.wav", true, 0.5, 0, &instrument3);
    statePointer->instruments.push_back(instrument3);
    Instrument instrument4;
    loadInstrument("/data/rim.wav", true, 0.5, 0, &instrument4);
    statePointer->instruments.push_back(instrument4);
    Instrument instrument5;
    loadInstrument("/data/bass-C.wav", true, 0.25, 0, &instrument5);
    statePointer->instruments.push_back(instrument5);
    Instrument instrument6;
    loadInstrument("/data/hhc.wav", true, 0.5, 0, &instrument6);
    statePointer->instruments.push_back(instrument6);
    Instrument instrument7;
    loadInstrument("/data/hho.wav", true, 0.5, 0, &instrument7);
    statePointer->instruments.push_back(instrument7);
    Instrument instrument8;
    loadInstrument("/data/guitar-AM.wav", true, 0.5, 0, &instrument8);
    statePointer->instruments.push_back(instrument8);
    Instrument instrument9;
    loadInstrument("/data/flute-X.wav", true, 0.5, 0, &instrument9);
    statePointer->instruments.push_back(instrument9);
    Instrument instrument10;
    loadInstrument("/data/trumpet-C.wav", true, 0.5, 0, &instrument10);
    statePointer->instruments.push_back(instrument10);

    statePointer->currentStaveIndex = 0;
    statePointer->currentOctaveIndex = 0;

    statePointer->currentPartIndex = 0;
    statePointer->currentPartInstrumentIndex = 0;
    // parts
    statePointer->parts = {};

    statePointer->currentStepIndex = 0;

    // part 1
    std::vector<std::vector<Step>> part1Steps = {};
    const int part1Staves = 2;
    for (int i = 0; i < STATE_PART_STEPS_LENGTH * part1Staves; i++)
    {
        switch (i)
        {
        case 0:
            part1Steps.push_back({{0, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 2:
            part1Steps.push_back({});
            break;
        case 4:
            part1Steps.push_back({{1, 0, 0.9}, {4, 2, 0.9}});
            break;
        case 6:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 8:
            part1Steps.push_back({{4, 3, 0.9}});
            break;
        case 10:
            part1Steps.push_back({{1, 0, 0.9}});
            break;
        case 12:
            part1Steps.push_back({{1, 0, 0.9}, {4, 5, 0.9}});
            break;
        case 14:
            part1Steps.push_back({});
            break;

        case 16:
            part1Steps.push_back({{0, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 18:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 20:
            part1Steps.push_back({{1, 0, 0.9}, {4, 2, 0.9}});
            break;
        case 22:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 24:
            part1Steps.push_back({{4, 3, 0.9}});
            break;
        case 26:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 28:
            part1Steps.push_back({{1, 0, 0.9}, {4, 5, 0.9}});
            break;
        case 29:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        // case 30:
        //     part1Steps.push_back({{0, 0, 0.9}});
        //     break;
        default:
            part1Steps.push_back({});
            break;
        }
    }
    Part part1 = {part1Staves, part1Steps};
    statePointer->parts.push_back(part1);

    // part 2
    std::vector<std::vector<Step>> part2Steps = {};
    const int part2Staves = 2;
    for (int i = 0; i < STATE_PART_STEPS_LENGTH * part2Staves; i++)
    {
        switch (i)
        {
        case 0:
            part2Steps.push_back({{0, 0, 0.9}, {6, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 2:
            part2Steps.push_back({{3, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 4:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}, {4, 2, 0.9}});
            break;
        case 6:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 8:
            part2Steps.push_back({{5, 0, 0.9}, {4, 3, 0.9}});
            break;
        case 10:
            part2Steps.push_back({{1, 0, 0.9}, {3, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 12:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}, {4, 5, 0.9}});
            break;
        case 14:
            part2Steps.push_back({{3, 0, 0.9}, {4, 7, 0.9}});
            break;

        case 16:
            part2Steps.push_back({{0, 0, 0.9}, {5, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 18:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 20:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}, {4, 2, 0.9}});
            break;
        case 22:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 24:
            part2Steps.push_back({{5, 0, 0.9}, {4, 3, 0.9}});
            break;
        case 26:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}, {4, 0, 0.9}});
            break;
        case 28:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}, {4, 5, 0.9}});
            break;
        case 29:
            part2Steps.push_back({{0, 0, 0.9}});
            break;
        case 30:
            part2Steps.push_back({{3, 0, 0.9}, {4, 7, 0.9}});
            break;
        default:
            part2Steps.push_back({});
            break;
        }
    }
    Part part2 = {part2Staves, part2Steps};
    statePointer->parts.push_back(part2);

    statePointer->isPlaying = false;

    return true;
}
