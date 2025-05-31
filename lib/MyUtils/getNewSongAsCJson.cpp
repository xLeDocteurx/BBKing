#include <vector>
#include <string>

#include <cJSON.h>

#include <MyUtils.h>
#include <Defs.h>

// TODO : Make it take a blank cjson object pointer as input
// void getNewSongAsCJson(State *statePointer, cJSON *cjsonObjectPointer)
void getNewSongAsCJson(State *statePointer, cJSON *cjsonObjectPointer, int newSongIndex)
{
    // TODO : error handling
    cJSON_AddNumberToObject(cjsonObjectPointer, "masterGain", 1.0);
    cJSON_AddStringToObject(cjsonObjectPointer, "songName", "New song");
    cJSON_AddNumberToObject(cjsonObjectPointer, "songTempo", 120);

    cJSON_AddNumberToObject(cjsonObjectPointer, "currentModeIndex", 0);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentSelectedStepIndex", 0);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentSongIndex", newSongIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentPartIndex", 0);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentPartInstrumentIndex", 0);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentStaveIndex", 0);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentOctaveIndex", 0);

    // TODO : TODO
    cJSON *instrumentsArray = cJSON_AddArrayToObject(cjsonObjectPointer, "instruments");
    for (int i = 0; i < 10; i++)
    {
        cJSON *cJsonInstrumentObject = cJSON_CreateObject();

        cJSON *cJsonSampleObject = cJSON_AddObjectToObject(cJsonInstrumentObject, "sample");
        cJSON_AddStringToObject(cJsonSampleObject, "filePath", "/data/kick.wav");
        cJSON_AddBoolToObject(cJsonSampleObject, "isMono", true);
        // TODO : real size
        cJSON_AddNumberToObject(cJsonSampleObject, "fileSize", 19000);

        cJSON_AddBoolToObject(cJsonInstrumentObject, "isSolo", false);
        cJSON_AddBoolToObject(cJsonInstrumentObject, "isMuted", false);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "volume", 0.5);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "pitch", 0);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "startPosition", 0.0);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "endPosition", 1.0);
        cJSON_AddBoolToObject(cJsonInstrumentObject, "isReverse", false);

        cJSON_AddItemToArray(instrumentsArray, cJsonInstrumentObject);
    }

    // TODO : TODO
    cJSON *partsArray = cJSON_AddArrayToObject(cjsonObjectPointer, "parts");
    cJSON *cJsonPartObject = cJSON_CreateObject();

    cJSON_AddNumberToObject(cJsonPartObject, "staves", 1);

    cJSON *stepsArray = cJSON_AddArrayToObject(cJsonPartObject, "steps");
    for (int j = 0; j < STATE_PART_STEPS_LENGTH; j++)
    {
        cJSON *stepArray = cJSON_CreateArray();
        cJSON_AddItemToArray(stepsArray, stepArray);
    }

    cJSON_AddItemToArray(partsArray, cJsonPartObject);
}
