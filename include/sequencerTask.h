#include <State.h>

TaskHandle_t sequencerTaskHandle;

void sequencerTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
        // play_wav_file("/data/flo1.wav");
        // vTaskDelay(pdMS_TO_TICKS(1000));
        // // play_wav_file("/data/flo2.wav");
        // // vTaskDelay(pdMS_TO_TICKS(1000));
        // play_wav_file("/data/flo3.wav");
        // vTaskDelay(pdMS_TO_TICKS(1000));
        // play_wav_file("/data/amenbreak.wav");
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // xTaskCreatePinnedToCore(play_wav_fileTask, "play_wav_fileTask", 8192, (void *)"/data/amenbreak.wav", 10, &play_wav_fileTaskHandle, 1);
        // vTaskDelay(pdMS_TO_TICKS(5000));

        if (statePointer->isPlaying)
        {
            // printf("tic\n");
            // printf("statePointer->currentStepIndex : %i\n", statePointer->currentStepIndex);
            // printf("statePointer->global.songs[%i].parts[%i].drumRack.steps[%i].size() : %i\n", currentSongIndex, currentPartIndex, statePointer->currentStepIndex, statePointer->global.songs[currentSongIndex].parts[currentPartIndex].drumRack.steps[statePointer->currentStepIndex].size());
            for (int stepContentIndex = 0; stepContentIndex < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); stepContentIndex++)
            // for (int stepContentIndex = 0; stepContentIndex < STATE_PART_STEPS_LENGTH * statePointer->parts[statePointer->currentPartIndex].staves; stepContentIndex++)
            {

                const int stepInstrumentSampleIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][stepContentIndex];
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
                // printf("play : %s\n", statePointer->samples[sampleFileRefIndex].filePath);
                statePointer->samples[sampleFileRefIndex].isPlaying = true;
                statePointer->samples[sampleFileRefIndex].bufferSamplesReadCounter = 0;
            }

            statePointer->currentStepIndex += 1;
            // if (statePointer->currentStepIndex >= statePointer->songBeats * statePointer->songMeasures * statePointer->parts[statePointer->currentPartIndex].staves)
            // if (statePointer->currentStepIndex >= STATE_PART_STEPS_LENGTH * statePointer->parts[statePointer->currentPartIndex].staves)
            if (statePointer->currentStepIndex >= STATE_PART_STEPS_LENGTH)
            {
                statePointer->currentStepIndex = 0;
            }
        }

        // vTaskDelay(pdMS_TO_TICKS(60000 / statePointer->csongTempo / statePointer->songBeats));
        vTaskDelay(pdMS_TO_TICKS(60000 / statePointer->songTempo / 4));
        // vTaskDelay(pdMS_TO_TICKS(60000 / statePointer->songTempo));
    }
    vTaskDelete(sequencerTaskHandle);
}
