#include <State.h>

TaskHandle_t sequencerTaskHandle;

void sequencerTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
        if (statePointer->isPlaying)
        {
            for (int stepContentIndex = 0; stepContentIndex < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size() * statePointer->parts[statePointer->currentPartIndex].staves; stepContentIndex++)
            {
                const int stepInstrumentSampleIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex];
                // // Interruption group for sample 6 and 7
                // if (stepInstrumentSampleIndex == 5 && statePointer->samples[6].isPlaying)
                // {
                //     statePointer->samples[6].isPlaying = false;
                //     statePointer->samples[6].bufferSamplesReadCounter = 0;
                // }
                // else if (stepInstrumentSampleIndex == 6 && statePointer->samples[5].isPlaying)
                // {
                //     statePointer->samples[5].isPlaying = false;
                //     statePointer->samples[5].bufferSamplesReadCounter = 0;
                // }

                int sampleFileRefIndex = 0;
                switch (stepInstrumentSampleIndex)
                {
                case 0:
                    sampleFileRefIndex = statePointer->drumRackSampleFileRefIndex1;
                    break;
                case 1:
                    sampleFileRefIndex = statePointer->drumRackSampleFileRefIndex2;
                    break;
                case 2:
                    sampleFileRefIndex = statePointer->drumRackSampleFileRefIndex3;
                    break;
                case 3:
                    sampleFileRefIndex = statePointer->drumRackSampleFileRefIndex4;
                    break;
                case 4:
                    sampleFileRefIndex = statePointer->drumRackSampleFileRefIndex5;
                    break;
                case 5:
                    sampleFileRefIndex = statePointer->drumRackSampleFileRefIndex6;
                    break;
                case 6:
                    sampleFileRefIndex = statePointer->drumRackSampleFileRefIndex7;
                    break;

                default:
                    break;
                }
                // printf("start %i/%i : %s\n", stepInstrumentSampleIndex, sampleFileRefIndex, statePointer->samples[sampleFileRefIndex].filePath);
                // printf("0 : %s\n", statePointer->samples[0].filePath);
                // printf("1 : %s\n", statePointer->samples[1].filePath);
                // printf("2 : %s\n", statePointer->samples[2].filePath);
                statePointer->samples[sampleFileRefIndex].isPlaying = true;
                statePointer->samples[sampleFileRefIndex].bufferSamplesReadCounter = 0;
            }

            statePointer->currentStepIndex += 1;
            if (statePointer->currentStepIndex >= STATE_PART_STEPS_LENGTH)
            {
                statePointer->currentStepIndex = 0;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(60000 / statePointer->songTempo / 4));
    }
    vTaskDelete(sequencerTaskHandle);
}
