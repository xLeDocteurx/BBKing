// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleStepIsReverse(State *statePointer, std::string actionParameters)
{
    printf("UPDATEINSTRUMENTSAMPLESTEPISREVERSE\n");
    int xxxIndex = 0;
    for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
    {
        int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
        if (stepInstrumentIndex == statePointer->currentPartInstrumentIndex)
        {
            xxxIndex = i;
        }
    }
    statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].isReverse = stoi(actionParameters);
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLESTEPISREVERSE@" + actionParameters).c_str());
}
