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
                const int stepInstrumentSampleIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex].instrumentIndex;
                int sampleIndex = getInstrumentSampleIndex(statePointer, stepInstrumentSampleIndex);

                // printf("start %i/%i : %s\n", stepInstrumentSampleIndex, sampleIndex, statePointer->samples[sampleIndex].filePath);
                statePointer->samples[sampleIndex].isPlaying = true;
                statePointer->samples[sampleIndex].bufferSamplesReadCounter = 0;
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
