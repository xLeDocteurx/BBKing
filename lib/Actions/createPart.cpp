// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void createPart(State *statePointer, std::string actionParameters)
{
    // int desiredIndex = stoi(actionParameters);
    // if (desiredIndex >= 0 && desiredIndex >= statePointer->parts.size())
    // {
    //     statePointer->currentPartIndex = desiredIndex;
    std::vector<std::vector<Step>> newPartSteps = {};
    const int newPartStaves = 1;
    for (int i = 0; i < STATE_PART_STEPS_LENGTH * newPartStaves; i++)
    {
        switch (i)
        {
        default:
            newPartSteps.push_back({});
            break;
        }
    }
    Part newPart = {newPartStaves, newPartSteps};
    statePointer->parts.push_back(newPart);
    statePointer->currentPartIndex += 1;
    statePointer->currentStaveIndex = 0;
    // }
    broadcast_ws_message(("CREATEPART@" + actionParameters).c_str());
}
