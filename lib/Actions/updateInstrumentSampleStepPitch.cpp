// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleStepPitch(State *statePointer, std::string actionParameters)
{
    printf("UPDATEINSTRUMENTSAMPLESTEPPITCH\n");
    int xxxIndex = 0;
    for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
    {
        // int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
        if (statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].get()->instrumentIndex == statePointer->currentPartInstrumentIndex)
        {
            xxxIndex = i;
        }
    }
    printf("stoi(actionParameters) : %i\n", stoi(actionParameters));
    // statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].pitch = stoi(actionParameters);

    printf("pitch BEFORE : %i\n", statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex][xxxIndex].get()->pitch);
    statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex][xxxIndex].get()->pitch = stoi(actionParameters);
    printf("pitch AFTER : %i\n", statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex][xxxIndex].get()->pitch);
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLESTEPPITCH@" + actionParameters).c_str());
}
