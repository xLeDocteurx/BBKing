#include <vector>
#include <string>

#include <esp_psram.h>
#include <esp_system.h>
#include <driver/spi_master.h>

#include <GlobalVars.h>

#include <readJsonFile.h>
#include <Sample.h>
#include <State.h>

void freeFile(void *filePointer)
{
    free(filePointer);
}

bool loadFile(char *filePath, float volume, int pitch, Sample *samplePointer)
{
    printf("loadFile(%s);\n", filePath);

    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", filePath);
        return false;
    }
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file) - 44;
    fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)

    int16_t *fileBufferPointer = (int16_t *)malloc(fileSize);

    size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t), fileSize / sizeof(int16_t), file);

    fclose(file);

    *samplePointer = {filePath, volume, pitch, fileSize, fileBufferPointer, false, 0};
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
    statePointer->currentSongIndex = 0;
    statePointer->songName = "Demo song";
    statePointer->songTempo = 120;

    statePointer->samples = {};

    Sample sample1;
    loadFile("/data/kick.wav", 0.5, 0, &sample1);
    statePointer->samples.push_back(sample1);
    Sample sample2;
    loadFile("/data/snare.wav", 0.5, 0, &sample2);
    statePointer->samples.push_back(sample2);
    Sample sample3;
    loadFile("/data/clap.wav", 0.5, 0, &sample3);
    statePointer->samples.push_back(sample3);
    Sample sample4;
    loadFile("/data/rim.wav", 0.5, 0, &sample4);
    statePointer->samples.push_back(sample4);
    Sample sample5;
    // loadFile("/data/bass-C.wav", 0.5, -12, &sample5);
    // loadFile("/data/bass-C.wav", 0.5, 12, &sample5);
    loadFile("/data/bass-C.wav", 0.5, 0, &sample5);
    statePointer->samples.push_back(sample5);
    Sample sample6;
    loadFile("/data/hhc.wav", 0.5, 0, &sample6);
    statePointer->samples.push_back(sample6);
    Sample sample7;
    loadFile("/data/hho.wav", 0.5, 0, &sample7);
    statePointer->samples.push_back(sample7);

    // drumRack
    statePointer->drumRackSampleFileRefIndex1 = 0;
    statePointer->drumRackSampleFileRefIndex2 = 1;
    statePointer->drumRackSampleFileRefIndex3 = 2;
    statePointer->drumRackSampleFileRefIndex4 = 3;
    statePointer->drumRackSampleFileRefIndex5 = 4;
    statePointer->drumRackSampleFileRefIndex6 = 5;
    statePointer->drumRackSampleFileRefIndex7 = 6;
    // slicer
    // statePointer->slicerSampleFileRefIndex;
    // keyboard
    // statePointer->keyboardSampleFileRefIndex = 5;

    statePointer->currentPartIndex = 0;
    // statePointer->currentPartInstrument = INTRUMENT_DRUMRACK1;
    statePointer->currentPartInstrument = 0;
    // parts
    statePointer->parts = {};

    statePointer->currentStepIndex = 0;
    // parts steps
    std::vector<std::vector<int>> part1Steps = {};
    const int part1Staves = 1;
    for (int i = 0; i < STATE_PART_STEPS_LENGTH * part1Staves; i++)
    {
        switch (i)
        {
        case 0:
            part1Steps.push_back({0});
            break;
        case 2:
            part1Steps.push_back({});
            break;
        case 4:
            part1Steps.push_back({0,2});
            break;
        case 6:
            part1Steps.push_back({});
            break;
        case 8:
            part1Steps.push_back({0});
            break;
        case 10:
            part1Steps.push_back({});
            break;
        case 12:
            part1Steps.push_back({0,2});
            break;
        case 14:
            part1Steps.push_back({});
            break;
        default:
            part1Steps.push_back({});
            break;
        }
    }
    Part part1 = {part1Staves, part1Steps};
    statePointer->parts.push_back(part1);

    statePointer->isPlaying = false;

    return true;
}
