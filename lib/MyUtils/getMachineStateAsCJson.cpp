#include <vector>
#include <string>

#include <cJSON.h>

#include <MyUtils.h>
#include <Defs.h>

// TODO : Make it take a blank cjson object pointer as input
void getMachineStateAsCJson(State *statePointer, cJSON *cjsonObjectPointer)
{
    // TODO : error handling
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentModeIndex", statePointer->currentModeIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentSelectedStepIndex", statePointer->currentSelectedStepIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentSongIndex", statePointer->currentSongIndex);
    cJSON_AddStringToObject(cjsonObjectPointer, "songName", statePointer->songName);
    cJSON_AddNumberToObject(cjsonObjectPointer, "songTempo", statePointer->songTempo);

    // TODO : TODO
    cJSON *instrumentsArray = cJSON_AddArrayToObject(cjsonObjectPointer, "instruments");
    for (int i = 0; i < statePointer->instruments.size(); i++)
    {
        cJSON *cJsonInstrumentObject = cJSON_CreateObject();

        cJSON *cJsonSampleObject = cJSON_AddObjectToObject(cJsonInstrumentObject, "sample");
        cJSON_AddStringToObject(cJsonSampleObject, "filePath", statePointer->instruments[i].sample.filePath);
        cJSON_AddBoolToObject(cJsonSampleObject, "isMono", statePointer->instruments[i].sample.isMono);
        cJSON_AddNumberToObject(cJsonSampleObject, "fileSize", statePointer->instruments[i].sample.fileSize);

        cJSON_AddBoolToObject(cJsonInstrumentObject, "isSolo", statePointer->instruments[i].isSolo);
        cJSON_AddBoolToObject(cJsonInstrumentObject, "isMuted", statePointer->instruments[i].isMuted);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "volume", statePointer->instruments[i].volume);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "pitch", statePointer->instruments[i].pitch);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "startPosition", statePointer->instruments[i].startPosition);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "endPosition", statePointer->instruments[i].endPosition);
        cJSON_AddBoolToObject(cJsonInstrumentObject, "isReverse", statePointer->instruments[i].isReverse);

        cJSON_AddItemToArray(instrumentsArray, cJsonInstrumentObject);
    }

    cJSON_AddNumberToObject(cjsonObjectPointer, "currentPartIndex", statePointer->currentPartIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentPartInstrumentIndex", statePointer->currentPartInstrumentIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentStaveIndex", statePointer->currentStaveIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentOctaveIndex", statePointer->currentOctaveIndex);

    // TODO : TODO
    cJSON *partsArray = cJSON_AddArrayToObject(cjsonObjectPointer, "parts");
    for (int i = 0; i < statePointer->parts.size(); i++)
    {
        cJSON *cJsonPartObject = cJSON_CreateObject();

        cJSON_AddNumberToObject(cJsonPartObject, "staves", statePointer->parts[i].staves);

        std::string stepsString = "[";
        cJSON *stepsArray = cJSON_AddArrayToObject(cJsonPartObject, "steps");
        for (int j = 0; j < statePointer->parts[i].steps.size(); j++)
        {
            cJSON *stepArray = cJSON_CreateArray();

            std::string stepString = "[";
            for (int k = 0; k < statePointer->parts[i].steps[j].size(); k++)
            {
                cJSON *stepItem = cJSON_CreateObject();
                cJSON_AddNumberToObject(stepItem, "instrumentIndex", statePointer->parts[i].steps[j][k].instrumentIndex);
                cJSON_AddNumberToObject(stepItem, "volume", statePointer->parts[i].steps[j][k].volume);
                cJSON_AddNumberToObject(stepItem, "pitch", statePointer->parts[i].steps[j][k].pitch);
                cJSON_AddNumberToObject(stepItem, "startPosition", statePointer->parts[i].steps[j][k].startPosition);
                cJSON_AddNumberToObject(stepItem, "endPosition", statePointer->parts[i].steps[j][k].endPosition);
                cJSON_AddBoolToObject(stepItem, "isReverse", statePointer->parts[i].steps[j][k].isReverse);

                cJSON_AddItemToArray(stepArray, stepItem);
            }

            cJSON_AddItemToArray(stepsArray, stepArray);
        }

        cJSON_AddItemToArray(partsArray, cJsonPartObject);
    }
}
