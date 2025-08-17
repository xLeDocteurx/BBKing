#include <vector>
#include <string>

#include <cJSON.h>

#include <MyUtils.h>
#include <Defs.h>

// TODO : Make it take a blank cjson object pointer as input
void getMachineStateAsCJson(State *statePointer, cJSON *cjsonObjectPointer)
{
    // TODO : error handling
    cJSON_AddNumberToObject(cjsonObjectPointer, "masterGain", statePointer->masterGain);
    cJSON_AddStringToObject(cjsonObjectPointer, "songName", statePointer->songName);
    printf("getMachineStateAsCJson songName : %s\n", statePointer->songName);
    cJSON_AddNumberToObject(cjsonObjectPointer, "songTempo", statePointer->songTempo);
    printf("getMachineStateAsCJson songTempo : %i\n", statePointer->songTempo);

    cJSON_AddNumberToObject(cjsonObjectPointer, "currentMode", statePointer->currentMode);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentSelectedStepIndex", statePointer->currentSelectedStepIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentSongIndex", statePointer->currentSongIndex);
    printf("getMachineStateAsCJson currentSongIndex : %i\n", statePointer->currentSongIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentPartIndex", statePointer->currentPartIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentPartInstrumentIndex", statePointer->currentPartInstrumentIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentStaveIndex", statePointer->currentStaveIndex);
    cJSON_AddNumberToObject(cjsonObjectPointer, "currentOctaveIndex", statePointer->currentOctaveIndex);

    // TODO : TODO
    cJSON *instrumentsArray = cJSON_AddArrayToObject(cjsonObjectPointer, "instruments");

    printf("-------------------------\n");
    printf("-------------------------\n");
    printf("statePointer->instruments.size() %i\n", statePointer->instruments.size());

    for (int i = 0; i < statePointer->instruments.size(); i++)
    {
        cJSON *cJsonInstrumentObject = cJSON_CreateObject();

        cJSON_AddNumberToObject(cJsonInstrumentObject, "type", statePointer->instruments[i]->type);
        cJSON_AddBoolToObject(cJsonInstrumentObject, "isSolo", statePointer->instruments[i]->isSolo);
        cJSON_AddBoolToObject(cJsonInstrumentObject, "isMuted", statePointer->instruments[i]->isMuted);
        // TODO : Add isPlaying ? ( quid du chargement/save ? )
        cJSON_AddNumberToObject(cJsonInstrumentObject, "volume", statePointer->instruments[i]->volume);
        cJSON_AddNumberToObject(cJsonInstrumentObject, "pitch", statePointer->instruments[i]->pitch);

        // TODO : Same thing in save state
        if (statePointer->instruments[i]->type == DRUM_RACK)
        {
            DrumRack *drumRack = static_cast<DrumRack *>(statePointer->instruments[i].get());
            cJSON *cJsonSampleObject = cJSON_AddObjectToObject(cJsonInstrumentObject, "sample");
            cJSON_AddStringToObject(cJsonSampleObject, "filePath", drumRack->sample.filePath);
            cJSON_AddBoolToObject(cJsonSampleObject, "isMono", drumRack->sample.isMono);
            cJSON_AddNumberToObject(cJsonSampleObject, "fileSize", drumRack->sample.fileSize);
            cJSON_AddBoolToObject(cJsonInstrumentObject, "isReverse", drumRack->isReverse);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "startPosition", drumRack->startPosition);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "endPosition", drumRack->endPosition);
        }
        else if (statePointer->instruments[i]->type == SAMPLER)
        {
            Sampler *sampler = static_cast<Sampler *>(statePointer->instruments[i].get());
            cJSON *cJsonSampleObject = cJSON_AddObjectToObject(cJsonInstrumentObject, "sample");
            cJSON_AddStringToObject(cJsonSampleObject, "filePath", sampler->sample.filePath);
            cJSON_AddBoolToObject(cJsonSampleObject, "isMono", sampler->sample.isMono);
            cJSON_AddNumberToObject(cJsonSampleObject, "fileSize", sampler->sample.fileSize);
            cJSON_AddBoolToObject(cJsonInstrumentObject, "isLooping", sampler->isLooping);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "attackPosition", sampler->attackPosition);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "decayPosition", sampler->decayPosition);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "sustainPosition", sampler->sustainPosition);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "releasePosition", sampler->releasePosition);
        }
        else if (statePointer->instruments[i]->type == SYNTH)
        {
            Synth *synth = static_cast<Synth *>(statePointer->instruments[i].get());
            cJSON_AddNumberToObject(cJsonInstrumentObject, "osc1WaveFormType", synth->osc1WaveFormType);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "osc2WaveFormType", synth->osc2WaveFormType);
            cJSON_AddNumberToObject(cJsonInstrumentObject, "osc3WaveFormType", synth->osc3WaveFormType);
        }

        cJSON_AddItemToArray(instrumentsArray, cJsonInstrumentObject);
    }

    printf("cJSON_GetArraySize(instrumentsArray) %i\n", cJSON_GetArraySize(instrumentsArray));
    printf("-------------------------\n");
    printf("-------------------------\n");

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
                cJSON_AddNumberToObject(stepItem, "instrumentIndex", statePointer->parts[i].steps[j][k].get()->instrumentIndex);
                cJSON_AddNumberToObject(stepItem, "volume", statePointer->parts[i].steps[j][k].get()->volume);
                cJSON_AddNumberToObject(stepItem, "pitch", statePointer->parts[i].steps[j][k].get()->pitch);

                if (statePointer->instruments[statePointer->parts[i].steps[j][k].get()->instrumentIndex].get()->type == DRUM_RACK_STEP)
                {
                    DrumRackStep *drumStep = static_cast<DrumRackStep *>(statePointer->parts[i].steps[j][k].get());
                    cJSON_AddBoolToObject(stepItem, "isReverse", drumStep->isReverse);
                    cJSON_AddNumberToObject(stepItem, "startPosition", drumStep->startPosition);
                    cJSON_AddNumberToObject(stepItem, "endPosition", drumStep->endPosition);
                }
                else if (statePointer->instruments[statePointer->parts[i].steps[j][k].get()->instrumentIndex].get()->type == SAMPLER_STEP)
                {
                    // SamplerStep *samplerStep = static_cast<SamplerStep *>(&statePointer->parts[i].steps[j][k]);
                    // cJSON_AddBoolToObject(stepItem, "isLooping", samplerStep->isLooping);
                    // cJSON_AddNumberToObject(stepItem, "loopStartPosition", samplerStep->loopStartPosition);
                    // cJSON_AddNumberToObject(stepItem, "loopEndPosition", samplerStep->loopEndPosition);
                    // cJSON_AddNumberToObject(stepItem, "attackPosition", samplerStep->attackPosition);
                    // cJSON_AddNumberToObject(stepItem, "decayPosition", samplerStep->decayPosition);
                    // cJSON_AddNumberToObject(stepItem, "sustainPosition", samplerStep->sustainPosition);
                    // cJSON_AddNumberToObject(stepItem, "releasePosition", samplerStep->releasePosition);
                }
                else if (statePointer->instruments[statePointer->parts[i].steps[j][k].get()->instrumentIndex].get()->type == SYNTH_STEP)
                {
                }

                cJSON_AddItemToArray(stepArray, stepItem);
            }

            cJSON_AddItemToArray(stepsArray, stepArray);
        }

        cJSON_AddItemToArray(partsArray, cJsonPartObject);
    }
}
