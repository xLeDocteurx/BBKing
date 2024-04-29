#include <Tasks.h>

#include <Defs.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// TaskHandle_t sequencerTaskHandle;

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
                // int sampleIndex = statePointer->instruments[stepInstrumentIndex].sampleFileRefIndex;

                // Interruption groups
                switch (stepInstrumentIndex)
                {
                case 5:
                    // Interruption group for sample 6 and 7
                    if (statePointer->instruments[6].isPlaying)
                    {
                        statePointer->instruments[6].isPlaying = false;
                        statePointer->instruments[6].bufferSamplesReadCounter = 0;
                    }
                    break;
                case 6:
                    // Interruption group for sample 6 and 7
                    if (statePointer->instruments[5].isPlaying)
                    {
                        statePointer->instruments[5].isPlaying = false;
                        statePointer->instruments[5].bufferSamplesReadCounter = 0;
                    }
                    break;
                default:
                    break;
                }

                // printf("start %i/%i : %s\n", stepInstrumentSampleIndex, sampleIndex, statePointer->instruments[stepInstrumentIndex].filePath);
                statePointer->instruments[stepInstrumentIndex].isPlaying = true;
                statePointer->instruments[stepInstrumentIndex].bufferSamplesReadCounter = 0;
                statePointer->instruments[stepInstrumentIndex].startingStepVolume = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].volume;
                statePointer->instruments[stepInstrumentIndex].startingStepPitch = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].pitch;
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
    // TODO : WHAT THE FUCKc
    // vTaskDelete(sequencerTaskHandle);
}
