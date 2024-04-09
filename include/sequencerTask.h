#include <State.h>

TaskHandle_t sequencerTaskHandle;

void sequencerTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
        if (statePointer->isPlaying)
        {
            for (int stepContentIndex = 0; stepContentIndex < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); stepContentIndex++)
            {
                const int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].instrumentIndex;
                int sampleIndex = statePointer->instruments[stepInstrumentIndex].sampleFileRefIndex;

                // Interruption groups
                switch (stepInstrumentIndex)
                {
                case 5:
                    // Interruption group for sample 6 and 7
                    if (statePointer->samples[statePointer->instruments[6].sampleFileRefIndex].isPlaying)
                    {
                        statePointer->samples[statePointer->instruments[6].sampleFileRefIndex].isPlaying = false;
                        statePointer->samples[statePointer->instruments[6].sampleFileRefIndex].bufferSamplesReadCounter = 0;
                    }
                    break;
                case 6:
                    // Interruption group for sample 6 and 7
                    if (statePointer->samples[statePointer->instruments[5].sampleFileRefIndex].isPlaying)
                    {
                        statePointer->samples[statePointer->instruments[5].sampleFileRefIndex].isPlaying = false;
                        statePointer->samples[statePointer->instruments[5].sampleFileRefIndex].bufferSamplesReadCounter = 0;
                    }
                    break;
                default:
                    break;
                }

                // printf("start %i/%i : %s\n", stepInstrumentSampleIndex, sampleIndex, statePointer->samples[sampleIndex].filePath);
                statePointer->samples[sampleIndex].isPlaying = true;
                statePointer->samples[sampleIndex].bufferSamplesReadCounter = 0;
                statePointer->samples[sampleIndex].startingStepVolume = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].volume;
                statePointer->samples[sampleIndex].startingStepPitch = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].pitch;
            }

            // printf("%i\n", statePointer->currentStepIndex);
            statePointer->currentStepIndex += 1;
            if (statePointer->currentStepIndex >= STATE_PART_STEPS_LENGTH * statePointer->parts[statePointer->currentPartIndex].staves)
            {
                statePointer->currentStepIndex = 0;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(60000 / statePointer->songTempo / 4));
    }
    vTaskDelete(sequencerTaskHandle);
}
