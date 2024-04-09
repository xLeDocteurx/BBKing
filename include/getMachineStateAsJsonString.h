#include <string>

#include <State.h>

#ifndef GETMACHINESTATEASJSONSTRING_H
#define GETMACHINESTATEASJSONSTRING_H

void getMachineStateAsJsonString(State *statePointer, std::string *jsonStringPointer)
{

    std::string samplesString = "[";
    for (int i = 0; i < statePointer->samples.size(); i++)
    {
        std::string sampleString = "{\"filePath\": \"" + std::string(statePointer->samples[i].filePath) + "\", \"pitch\":" + std::to_string(statePointer->samples[i].pitch) + ", \"volume\":\"" + std::to_string(statePointer->samples[i].volume) + "\"}";
        if (i != 0)
        {
            samplesString += ",";
        }
        samplesString += sampleString;
    }
    samplesString += "]";

    std::string instrumentsString = "[";
    for (int i = 0; i < statePointer->instruments.size(); i++)
    {
        std::string instrumentString = "{\"sampleFileRefIndex\": " + std::to_string(statePointer->instruments[i].sampleFileRefIndex) + "}";
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
        ",\"samples\":" +
        samplesString +
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

#endif // GETMACHINESTATEASJSONSTRING_H
