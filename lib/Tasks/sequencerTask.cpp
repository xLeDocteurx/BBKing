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
                // int sampleIndex = statePointer->instruments[stepInstrumentIndex].sampleFileRefIndex;

                // Interruption groups
                switch (stepInstrumentIndex)
                {
                case 0:
                    // Interruption group for sample 1 and 2
                    if (statePointer->instruments[1].isPlaying)
                    {
                        statePointer->instruments[1].isPlaying = false;
                        statePointer->instruments[1].bufferSamplesReadCounter = 0;
                    }
                    break;
                case 1:
                    // Interruption group for sample 1 and 2
                    if (statePointer->instruments[0].isPlaying)
                    {
                        statePointer->instruments[0].isPlaying = false;
                        statePointer->instruments[0].bufferSamplesReadCounter = 0;
                    }
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

                statePointer->instruments[stepInstrumentIndex].startingStepVolume = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].volume;
                statePointer->instruments[stepInstrumentIndex].startingStepPitch = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].pitch;
                statePointer->instruments[stepInstrumentIndex].startingStepStartPosition = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].startPosition;
                statePointer->instruments[stepInstrumentIndex].startingStepEndPosition = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].endPosition;
                statePointer->instruments[stepInstrumentIndex].startingStepIsReverse = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].isReverse;

                int playbackStartPosition = statePointer->instruments[stepInstrumentIndex].sample.fileSize / 2 * ((statePointer->instruments[stepInstrumentIndex].startingStepStartPosition == 0.0) ? statePointer->instruments[stepInstrumentIndex].startPosition : statePointer->instruments[stepInstrumentIndex].startingStepStartPosition);
                // int playbackEndPosition = (statePointer->instruments[stepInstrumentIndex].startingStepEndPosition == 1.0) ? statePointer->instruments[stepInstrumentIndex].endPosition : statePointer->instruments[stepInstrumentIndex].startingStepEndPosition;
                // TODO : isReverse
                // bool playbackIsReverse = statePointer->instruments[stepInstrumentIndex].isReverse xor statePointer->instruments[stepInstrumentIndex].startingStepIsReverse;
                // printf("start %i/%i : %s\n", stepInstrumentSampleIndex, sampleIndex, statePointer->instruments[stepInstrumentIndex].filePath);
                statePointer->instruments[stepInstrumentIndex].isPlaying = true;
                statePointer->instruments[stepInstrumentIndex].bufferSamplesReadCounter = 0;
                // statePointer->instruments[stepInstrumentIndex].bufferSamplesReadCounter = round(statePointer->instruments[stepInstrumentIndex].sample.fileSize / (DMA_BITS_PER_SAMPLE / 8) * playbackStartPosition);
                // statePointer->instruments[stepInstrumentIndex].bufferSamplesReadCounter = round(statePointer->instruments[stepInstrumentIndex].sample.fileSize / sizeof(int16_t) * playbackStartPosition);
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
