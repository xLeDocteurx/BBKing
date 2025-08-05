// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void selectStave(State *statePointer, std::string actionParameters)
{
    int desiredStaveIndex = stoi(actionParameters);
    if (desiredStaveIndex >= 0 && desiredStaveIndex < statePointer->parts[statePointer->currentPartIndex].staves)
    {
        statePointer->currentStaveIndex = desiredStaveIndex;
    }
    broadcast_ws_message(("SELECTSTAVE@" + actionParameters).c_str());
}
