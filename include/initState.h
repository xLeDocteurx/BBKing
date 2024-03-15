#include <vector>
#include <string>

#include <esp_psram.h>
#include <esp_system.h>
#include <driver/spi_master.h>

#include <GlobalVars.h>

#include <readJsonFile.h>
#include <Sample.h>
#include <State.h>

void freeFileFromPSRAM(FILE *filePointer)
{
    free(filePointer);
}

// FILE *loadFileInPSRAM(char *filePath)
void loadFileInPSRAM(char *filePath)
{
    printf("loadFileInPSRAM(%s);\n", filePath);

    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        printf("Failed to open file\n");
        // return;
    }

    fseek(file, 0, SEEK_END);
    size_t fSize = ftell(file);
    // rewind(file);
    fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)

    int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE];
    // int16_t *buffer[fSize] = (int16_t **)ps_malloc(fSize);
    // int16_t *buffer[fSize] = (int16_t **)esp_hime(fSize);

    while (1)
    {
        // printf("4.51\n");
        size_t bytes_read = fread(buffer, sizeof(int16_t) / 2, fSize, file);
        if (bytes_read <= 0)
        {
            printf("stop(%s);\n", filePath);
            break; // End of file or error
        }
        size_t bytes_written; // Initialize bytes_written variable
        // printf("4.52\n");
        // i2s_write(I2S_NUM_0, buffer, bytes_read, NULL, portMAX_DELAY);

        i2s_write(I2S_NUM_0, buffer, bytes_read, &bytes_written, portMAX_DELAY);
        // writeToMasterBuffer(buffer);

        // printf("4.53\n");
    }
    // printf("5\n");

    fclose(file);
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
    FILE *sample1 = fopen("/data/kick.wav", "rb");
    if (sample1 == NULL)
    {
        printf("Failed to open file %s\n", "/data/kick.json");
        return false;
    }
    statePointer->samples.push_back({"/data/kick.wav", sample1, false, 0});
    FILE *sample2 = fopen("/data/snare.wav", "rb");
    if (sample2 == NULL)
    {
        printf("Failed to open file %s\n", "/data/snare.wav");
        return false;
    }
    statePointer->samples.push_back({"/data/snare.wav", sample2, false, 0});
    FILE *sample3 = fopen("/data/hithat.wav", "rb");
    if (sample3 == NULL)
    {
        printf("Failed to open file %s\n", "/data/hithat.wav");
        return false;
    }
    statePointer->samples.push_back({"/data/hithat.wav", sample3, false, 0});
    FILE *sample4 = fopen("/data/shake.wav", "rb");
    if (sample4 == NULL)
    {
        printf("Failed to open file %s\n", "/data/shake.wav");
        return false;
    }
    statePointer->samples.push_back({"/data/shake.wav", sample4, false, 0});
    FILE *sample5 = fopen("/data/tom1.wav", "rb");
    if (sample5 == NULL)
    {
        printf("Failed to open file %s\n", "/data/tom1.wav");
        return false;
    }
    statePointer->samples.push_back({"/data/tom1.wav", sample5, false, 0});
    FILE *sample6 = fopen("/data/tom2.wav", "rb");
    if (sample6 == NULL)
    {
        printf("Failed to open file %s\n", "/data/tom2.wav");
        return false;
    }
    statePointer->samples.push_back({"/data/tom2.wav", sample6, false, 0});
    FILE *sample7 = fopen("/data/flo1.wav", "rb");
    if (sample7 == NULL)
    {
        printf("Failed to open file %s\n", "/data/flo1.wav");
        return false;
    }
    statePointer->samples.push_back({"/data/flo1.wav", sample7, false, 0});

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
