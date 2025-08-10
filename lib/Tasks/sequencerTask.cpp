#include <Tasks.h>
#include <Defs.h>

#include <math.h>

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

                // Interruption groups
                switch (stepInstrumentIndex)
                {
                // case 0:
                //     // Interruption group for sample 1 and 2
                //     if (statePointer->instruments[1].isPlaying)
                //     {
                //         statePointer->instruments[1].isPlaying = false;
                //         statePointer->instruments[1].bufferSamplesReadCounter = 0;
                //     }
                //     break;
                // case 1:
                //     // Interruption group for sample 1 and 2
                //     if (statePointer->instruments[0].isPlaying)
                //     {
                //         statePointer->instruments[0].isPlaying = false;
                //         statePointer->instruments[0].bufferSamplesReadCounter = 0;
                //     }
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

                case 7:
                    // Interruption group for sample 7, 8 and 9
                    if (statePointer->instruments[8].isPlaying)
                    {
                        statePointer->instruments[8].isPlaying = false;
                        statePointer->instruments[8].bufferSamplesReadCounter = 0;
                    }
                    if (statePointer->instruments[9].isPlaying)
                    {
                        statePointer->instruments[9].isPlaying = false;
                        statePointer->instruments[9].bufferSamplesReadCounter = 0;
                    }
                    break;
                case 8:
                    // Interruption group for sample 7, 8 and 9
                    if (statePointer->instruments[7].isPlaying)
                    {
                        statePointer->instruments[7].isPlaying = false;
                        statePointer->instruments[7].bufferSamplesReadCounter = 0;
                    }
                    if (statePointer->instruments[9].isPlaying)
                    {
                        statePointer->instruments[9].isPlaying = false;
                        statePointer->instruments[9].bufferSamplesReadCounter = 0;
                    }
                    break;
                case 9:
                    // Interruption group for sample 7, 8 and 9
                    if (statePointer->instruments[7].isPlaying)
                    {
                        statePointer->instruments[7].isPlaying = false;
                        statePointer->instruments[7].bufferSamplesReadCounter = 0;
                    }
                    if (statePointer->instruments[8].isPlaying)
                    {
                        statePointer->instruments[8].isPlaying = false;
                        statePointer->instruments[8].bufferSamplesReadCounter = 0;
                    }
                    break;
                default:
                    break;
                }

                statePointer->instruments[stepInstrumentIndex].previousStepVolume = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].volume;
                statePointer->instruments[stepInstrumentIndex].previousStepPitch = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].pitch;
                statePointer->instruments[stepInstrumentIndex].previousStepStartPosition = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].startPosition;
                statePointer->instruments[stepInstrumentIndex].previousStepEndPosition = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].endPosition;
                statePointer->instruments[stepInstrumentIndex].previousStepIsReverse = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].isReverse;

                int playbackStartPositionInSample;
                if (statePointer->instruments[stepInstrumentIndex].isReverse xor statePointer->instruments[stepInstrumentIndex].previousStepIsReverse)
                {
                    statePointer->instruments[stepInstrumentIndex].bufferSamplesReadCounter = round(statePointer->instruments[stepInstrumentIndex].sample.fileSize / sizeof(int16_t) * ((statePointer->instruments[stepInstrumentIndex].previousStepEndPosition == 1.0) ? statePointer->instruments[stepInstrumentIndex].endPosition : statePointer->instruments[stepInstrumentIndex].previousStepEndPosition));
                }
                else
                {
                    statePointer->instruments[stepInstrumentIndex].bufferSamplesReadCounter = round(statePointer->instruments[stepInstrumentIndex].sample.fileSize / sizeof(int16_t) * ((statePointer->instruments[stepInstrumentIndex].previousStepStartPosition == 0.0) ? statePointer->instruments[stepInstrumentIndex].startPosition : statePointer->instruments[stepInstrumentIndex].previousStepStartPosition));
                }
                // printf("start %i/%i : %s\n", statePointer->currentStepIndex, stepInstrumentIndex, statePointer->instruments[stepInstrumentIndex].sample.filePath);

                statePointer->instruments[stepInstrumentIndex].isPlaying = true;
            }

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
