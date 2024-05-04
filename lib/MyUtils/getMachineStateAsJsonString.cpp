#include <MyUtils.h>
#include <string>

#include <Defs.h>

void getMachineStateAsJsonString(State *statePointer, std::string *jsonStringPointer)
{
    std::string instrumentsString = "[";
    for (int i = 0; i < statePointer->instruments.size(); i++)
    {
        std::string instrumentString =
            "{\"sample\": {\"filePath\":\"" +
            std::string(statePointer->instruments[i].sample.filePath) +
            "\",\"isMono\":" +
            (statePointer->instruments[i].sample.isMono ? "true" : "false") +
            ",\"fileSize\":" +
            std::to_string(statePointer->instruments[i].sample.fileSize) +
            // TODO : finish implementation of the instrument object properties
            "}" +
            ",\"isSolo\":" + (statePointer->instruments[i].isSolo ? "true" : "false") +
            ",\"isMuted\":" + (statePointer->instruments[i].isMuted ? "true" : "false") +

            ",\"volume\":" + std::to_string(statePointer->instruments[i].volume) +
            ",\"pitch\":" + std::to_string(statePointer->instruments[i].pitch) +
            "}";
        if (i != 0)
        {
            instrumentsString += ",";
        }
        instrumentsString += instrumentString;
    }
    instrumentsString += "]";

    std::string partsString = "[";
    for (int i = 0; i < statePointer->parts.size(); i++)
    {
        std::string partString = "{\"staves\":" + std::to_string(statePointer->parts[i].staves) + ",\"steps\":";
        std::string stepsString = "[";
        for (int j = 0; j < statePointer->parts[i].steps.size(); j++)
        {
            std::string stepString = "[";
            for (int k = 0; k < statePointer->parts[i].steps[j].size(); k++)
            {
                if (k != 0)
                {
                    stepString += ",";
                }
                // stepString += std::to_string(statePointer->parts[i].steps[j][k]);
                statePointer->parts[i].steps[j][k].pitch;
                statePointer->parts[i].steps[j][k].volume;
                stepString += "{\"instrumentIndex\":" + std::to_string(statePointer->parts[i].steps[j][k].instrumentIndex) + ",\"pitch\":" + std::to_string(statePointer->parts[i].steps[j][k].pitch) + ",\"volume\":" + std::to_string(statePointer->parts[i].steps[j][k].volume) + "}";
            }
            stepString += "]";

            if (j != 0)
            {
                stepsString += ",";
            }
            stepsString += stepString;
        }
        stepsString += "]";

        partString += stepsString;

        if (i != 0)
        {
            partsString += ",";
        }
        partsString += partString + "}";
    }
    partsString += "]";

    std::string jsonString =
        "{\"currentModeIndex\":" +
        std::to_string(statePointer->currentModeIndex) +
        ",\"currentSelectedStepIndex\":" +
        std::to_string(statePointer->currentSelectedStepIndex) +
        ",\"currentSongIndex\":" +
        std::to_string(statePointer->currentSongIndex) +
        ",\"songName\":\"" +
        statePointer->songName +
        "\",\"songTempo\":" +
        std::to_string(statePointer->songTempo) +
        ",\"instruments\":" +
        instrumentsString +
        ",\"currentPartIndex\":" +
        std::to_string(statePointer->currentPartIndex) +
        ",\"currentPartInstrumentIndex\":" +
        std::to_string(statePointer->currentPartInstrumentIndex) +
        ",\"currentStaveIndex\":" +
        std::to_string(statePointer->currentStaveIndex) +
        ",\"currentOctaveIndex\":" +
        std::to_string(statePointer->currentOctaveIndex) +
        ",\"parts\":" +
        partsString +
        "}";

    jsonStringPointer->append(jsonString);
}
