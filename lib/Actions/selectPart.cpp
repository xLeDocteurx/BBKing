// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void selectPart(State *statePointer, std::string actionParameters)
{
    int desiredIndex = stoi(actionParameters);
    if (desiredIndex >= 0 && desiredIndex < statePointer->parts.size())
    {
        statePointer->currentPartIndex = desiredIndex;
        statePointer->currentStaveIndex = 0;
    }
    broadcast_ws_message(("SELECTPART@" + actionParameters).c_str());
}
