// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>

void updateCurrentStaveIndex(State *statePointer, std::string actionParameters)
{
    statePointer->currentStaveIndex = stoi(actionParameters);
    broadcast_ws_message(("UPDATECURRENTSTAVEINDEX@" + actionParameters).c_str());
}
