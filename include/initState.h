#include <vector>
#include <string>

#include <esp_psram.h>
#include <esp_system.h>
#include <driver/spi_master.h>

#include <GlobalVars.h>

#include <readJsonFile.h>
#include <State.h>
#include <Step.h>
#include <Sample.h>

void freeFile(void *filePointer)
{
    free(filePointer);
}

bool loadFile(char *filePath, bool isMono, float volume, int pitch, Sample *samplePointer)
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

    // TODO : read header for mono detection

    *samplePointer = {filePath, isMono, volume, pitch, fileSize, fileBufferPointer, false, 0};
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
    statePointer->currentSongIndex = 0;
    statePointer->songName = "Demo song";
    statePointer->songTempo = 154;

    statePointer->samples = {};

    Sample sample1;
    loadFile("/data/kick.wav", true, 0.75, 0, &sample1);
    statePointer->samples.push_back(sample1);
    Sample sample2;
    loadFile("/data/snare.wav", true, 0.75, 0, &sample2);
    // loadFile("/data/clap.wav", true, 0.75, 0, &sample2);
    statePointer->samples.push_back(sample2);
    Sample sample3;
    loadFile("/data/clap.wav", true, 0.75, 0, &sample3);
    // loadFile("/data/rim.wav", true, 0.75, 0, &sample3);
    statePointer->samples.push_back(sample3);
    Sample sample4;
    loadFile("/data/rim.wav", true, 0.75, 0, &sample4);
    // loadFile("/data/hhc.wav", true, 0.75, 0, &sample4);
    statePointer->samples.push_back(sample4);
    Sample sample5;
    // loadFile("/data/bass-C.wav", true, 0.75, -12, &sample5);
    // loadFile("/data/bass-C.wav", true, 0.75, 12, &sample5);
    loadFile("/data/bass-C.wav", true, 0.75, 0, &sample5);
    // loadFile("/data/hho.wav", true, 0.75, 0, &sample5);
    statePointer->samples.push_back(sample5);
    Sample sample6;
    loadFile("/data/hhc.wav", true, 0.75, 0, &sample6);
    // loadFile("/data/flo1.wav", true, 0.75, 0, &sample6);
    statePointer->samples.push_back(sample6);
    Sample sample7;
    loadFile("/data/hho.wav", true, 0.75, 0, &sample7);
    // loadFile("/data/flo2.wav", true, 0.75, 0, &sample7);
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
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 2:
            part1Steps.push_back({});
            break;
        case 4:
            part1Steps.push_back({{1, 0, 0.9}});
            break;
        case 6:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 8:
            part1Steps.push_back({});
            break;
        case 10:
            part1Steps.push_back({{1, 0, 0.9}});
            break;
        case 12:
            part1Steps.push_back({{1, 0, 0.9}});
            break;
        case 14:
            part1Steps.push_back({});
            break;
        
        case 16:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 18:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 20:
            part1Steps.push_back({{1, 0, 0.9}});
            break;
        case 22:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 24:
            part1Steps.push_back({});
            break;
        case 26:
            part1Steps.push_back({{0, 0, 0.9}});
            break;
        case 28:
            part1Steps.push_back({{1, 0, 0.9}});
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
            part2Steps.push_back({{0, 0, 0.9}, {6, 0, 0.9}});
            break;
        case 2:
            part2Steps.push_back({{3, 0, 0.9}});
            break;
        case 4:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}});
            break;
        case 6:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}});
            break;
        case 8:
            part2Steps.push_back({{5, 0, 0.9}});
            break;
        case 10:
            part2Steps.push_back({{1, 0, 0.9}, {3, 0, 0.9}});
            break;
        case 12:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}});
            break;
        case 14:
            part2Steps.push_back({{3, 0, 0.9}});
            break;
        
        case 16:
            part2Steps.push_back({{0, 0, 0.9}, {5, 0, 0.9}});
            break;
        case 18:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}});
            break;
        case 20:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}});
            break;
        case 22:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}});
            break;
        case 24:
            part2Steps.push_back({{5, 0, 0.9}});
            break;
        case 26:
            part2Steps.push_back({{0, 0, 0.9}, {3, 0, 0.9}});
            break;
        case 28:
            part2Steps.push_back({{1, 0, 0.9}, {5, 0, 0.9}});
            break;
        case 29:
            part2Steps.push_back({{0, 0, 0.9}});
            break;
        case 30:
            part2Steps.push_back({{3, 0, 0.9}});
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
