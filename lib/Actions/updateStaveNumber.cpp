// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateStaveNumber(State *statePointer, std::string actionParameters)
{
    int desiredStaveNumber = stoi(actionParameters);
    // if (desiredStaveNumber > 0)
    // {
    if (statePointer->currentStaveIndex >= desiredStaveNumber)
    {
        statePointer->currentStaveIndex = desiredStaveNumber - 1;
    }
    statePointer->parts[statePointer->currentPartIndex].staves = desiredStaveNumber;

    statePointer->parts[statePointer->currentPartIndex].steps.resize(STATE_PART_STEPS_LENGTH * statePointer->parts[statePointer->currentPartIndex].staves, {});
    // }
    broadcast_ws_message(("UPDATESTAVENUMBER@" + actionParameters).c_str());
}
