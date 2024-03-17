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

bool loadFile(char *filePath, Sample *samplePointer)
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

    // size_t freePsramSize = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    // size_t freeInternalRamSize = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);

    int16_t *fileBufferPointer = (int16_t *)malloc(fileSize);

    // freePsramSize = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    // freeInternalRamSize = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);

    // size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t) / 2, fileSize, file);
    size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t), fileSize / sizeof(int16_t), file);

    fclose(file);

    *samplePointer = {"/data/kick.wav", fileSize, fileBufferPointer, false, 0};
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
    statePointer->songTempo = 151;

    statePointer->samples = {};

    Sample sample1;
    loadFile("/data/kick.wav", &sample1);
    statePointer->samples.push_back(sample1);
    Sample sample2;
    loadFile("/data/snare.wav", &sample2);
    statePointer->samples.push_back(sample2);
    Sample sample3;
    loadFile("/data/hithat.wav", &sample3);
    statePointer->samples.push_back(sample3);
    Sample sample4;
    loadFile("/data/shake.wav", &sample4);
    statePointer->samples.push_back(sample4);
    Sample sample5;
    loadFile("/data/tom1.wav", &sample5);
    statePointer->samples.push_back(sample5);
    Sample sample6;
    loadFile("/data/tom2.wav", &sample6);
    statePointer->samples.push_back(sample6);
    Sample sample7;
    loadFile("/data/flo1.wav", &sample7);
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
            // part1Steps.push_back({0, 2, 6});
            part1Steps.push_back({0});
            // part1Steps.push_back({0, 2});
            // part1Steps.push_back({0, 2, 5});
            // part1Steps.push_back({6});
            break;
        case 2:
            // part1Steps.push_back({0, 3});
            part1Steps.push_back({0});
            break;
        case 4:
            part1Steps.push_back({1});
            // part1Steps.push_back({1, 2});
            break;
        case 6:
            // part1Steps.push_back({0, 3});
            part1Steps.push_back({0});
            break;
        case 8:
            part1Steps.push_back({0});
            // part1Steps.push_back({0, 2});
            break;
        // case 10:
        //     // part1Steps.push_back({3});
        //     break;
        case 12:
            part1Steps.push_back({1});
            // part1Steps.push_back({1, 2});
            break;
        case 13:
            part1Steps.push_back({0});
            break;
        // case 14:
        //     // part1Steps.push_back({3});
        //     part1Steps.push_back({4});
        //     break;
        // case 15:
        //     // part1Steps.push_back({3});
        //     // part1Steps.push_back({4});
        //     part1Steps.push_back({5});
        //     // part1Steps.push_back({6});
        //     break;
        default:
            part1Steps.push_back({});
            break;
        }
    }
    Part part1 = {part1Staves, part1Steps};
    statePointer->parts.push_back(part1);

    // statePointer->_masterBuffer[PLAY_WAV_WAV_BUFFER_SIZE];
    // for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++)
    // {
    //     statePointer->_masterBuffer[i] = 0;
    // }
    statePointer->isPlaying = false;

    return true;
}
