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
                int sampleIndex = getInstrumentSampleIndex(statePointer, stepInstrumentIndex);

                // Interruption groups
                switch (stepInstrumentIndex)
                {
                case 5:
                    // Interruption group for sample 6 and 7
                    if (statePointer->samples[statePointer->drumRackSampleFileRefIndex7].isPlaying)
                    {
                        statePointer->samples[statePointer->drumRackSampleFileRefIndex7].isPlaying = false;
                        statePointer->samples[statePointer->drumRackSampleFileRefIndex7].bufferSamplesReadCounter = 0;
                    }
                    break;
                case 6:
                    // Interruption group for sample 6 and 7
                    if (statePointer->samples[statePointer->drumRackSampleFileRefIndex6].isPlaying)
                    {
                        statePointer->samples[statePointer->drumRackSampleFileRefIndex6].isPlaying = false;
                        statePointer->samples[statePointer->drumRackSampleFileRefIndex6].bufferSamplesReadCounter = 0;
                    }
                    break;
                default:
                    break;
                }

                // printf("start %i/%i : %s\n", stepInstrumentSampleIndex, sampleIndex, statePointer->samples[sampleIndex].filePath);
                statePointer->samples[sampleIndex].isPlaying = true;
                statePointer->samples[sampleIndex].bufferSamplesReadCounter = 0;
            }

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
