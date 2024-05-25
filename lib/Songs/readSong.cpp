#include <vector>
#include <string>

#include <cJSON.h>

#include <Defs.h>
#include <Songs.h>
#include <MyUtils.h>
#include <SDCard.h>

bool readSong(State *statePointer, int songIndex)
{
    std::string jsonString;
    bool readJsonFileRet = readJsonFile("/sdcard/songs.json", &jsonString);
    if (!readJsonFileRet)
    {
        printf("Failed to readJsonFile \"/sdcard/songs.json\"\n");
        return NULL;
    }
    // printf("-\n");
    // printf("-\n");
    // printf("-\n");
    // printf("%s\n", jsonString.c_str());
    // printf("-\n");
    // printf("-\n");
    // printf("-\n");

    // Parse JSON
    cJSON *root = cJSON_Parse(jsonString.c_str());
    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error before: %s\n", error_ptr);
        }
        printf("Failed to parse JSON\n");
        // free(jsonBuffer);
        return NULL;
    }

    // TODO : pushback error handling;
    cJSON *songArrayItem = cJSON_GetArrayItem(root, statePointer->currentSongIndex);

    cJSON *songName = cJSON_GetObjectItemCaseSensitive(songArrayItem, "songName");
    statePointer->songName = songName->valuestring;
    cJSON *songTempo = cJSON_GetObjectItemCaseSensitive(songArrayItem, "songTempo");
    statePointer->songTempo = songTempo->valueint;

    // Instruments
    cJSON *songInstruments = cJSON_GetObjectItemCaseSensitive(songArrayItem, "instruments");

    // int arraySize = cJSON_GetArraySize(root);
    // for (int i = 0; i < arraySize; i++)
    // TODO : cJsonForEach
    for (int i = 0; i < 10; i++)
    {
        // TODO : Error handling for loadInstrument and the rest
        Instrument instrument;
        cJSON *songInstrument = cJSON_GetArrayItem(songInstruments, i);
        cJSON *songInstrumentSample = cJSON_GetObjectItemCaseSensitive(songInstrument, "sample");
        cJSON *songInstrumentFilePath = cJSON_GetObjectItemCaseSensitive(songInstrumentSample, "filePath");
        cJSON *songInstrumentIsMono = cJSON_GetObjectItemCaseSensitive(songInstrumentSample, "isMono");
        cJSON *songInstrumentVolume = cJSON_GetObjectItemCaseSensitive(songInstrument, "volume");
        cJSON *songInstrumentPitch = cJSON_GetObjectItemCaseSensitive(songInstrument, "pitch");
        cJSON *songInstrumentStartPosition = cJSON_GetObjectItemCaseSensitive(songInstrument, "startPosition");
        cJSON *songInstrumentEndPosition = cJSON_GetObjectItemCaseSensitive(songInstrument, "endPosition");
        cJSON *songInstrumentIsReverse = cJSON_GetObjectItemCaseSensitive(songInstrument, "isReverse");
        loadInstrument(songInstrumentFilePath->valuestring, songInstrumentIsMono->valueint, (float)songInstrumentVolume->valuedouble, songInstrumentPitch->valueint, (float)songInstrumentStartPosition->valuedouble, (float)songInstrumentEndPosition->valuedouble, (bool)songInstrumentIsReverse->valueint, &instrument);
        statePointer->instruments.push_back(instrument);
    }

    // parts
    statePointer->parts = {};

    cJSON *songParts = cJSON_GetObjectItemCaseSensitive(songArrayItem, "parts");
    // cJSON *songInstrument = cJSON_GetArrayItem(songInstruments, i);
    int songPartsLength = cJSON_GetArraySize(songParts);
    // TODO : cJsonForEach
    for (int i = 0; i < songPartsLength; i++)
    {
        cJSON *songPart = cJSON_GetArrayItem(songParts, i);

        cJSON *songPartStaves = cJSON_GetObjectItemCaseSensitive(songPart, "staves");
        const int partStaves = songPartStaves->valueint;
        cJSON *songPartSteps = cJSON_GetObjectItemCaseSensitive(songPart, "steps");
        int songPartStepsLength = cJSON_GetArraySize(songPartSteps);
        std::vector<std::vector<Step>> partSteps = {};
        for (int j = 0; j < STATE_PART_STEPS_LENGTH * partStaves; j++)
        // for (int j = 0; j < songPartStepsLength; j++)
        {
            cJSON *songPartStep = cJSON_GetArrayItem(songPartSteps, j);
            int songPartStepContentLength = cJSON_GetArraySize(songPartStep);
            std::vector<Step> partStepContent = {};

            // printf("songPartStepContentLength : %i\n", songPartStepContentLength);
            for (int k = 0; k < songPartStepContentLength; k++)
            {
                cJSON *songPartStepContent = cJSON_GetArrayItem(songPartStep, k);
                cJSON *songPartStepContentInstrumentIndex = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "instrumentIndex");
                cJSON *songPartStepContentVolume = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "volume");
                cJSON *songPartStepContentPitch = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "pitch");
                cJSON *songPartStepContentStartPosition = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "startPosition");
                cJSON *songPartStepContentEndPosition = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "endPosition");
                cJSON *songPartStepContentIsReverse = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "isReverse");
                partStepContent.push_back({songPartStepContentInstrumentIndex->valueint, (float)songPartStepContentVolume->valuedouble, songPartStepContentPitch->valueint, (float)songPartStepContentStartPosition->valuedouble, (float)songPartStepContentEndPosition->valuedouble, (bool)songPartStepContentIsReverse->valueint});
            }

            partSteps.push_back(partStepContent);
        }

        Part part = {partStaves, partSteps};
        statePointer->parts.push_back(part);
    }

    return true;
}
