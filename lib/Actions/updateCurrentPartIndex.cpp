// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>

void updateCurrentPartIndex(State *statePointer, std::string actionParameters)
{
    statePointer->currentPartIndex = stoi(actionParameters);
    statePointer->currentStaveIndex = 0;
    broadcast_ws_message(("UPDATECURRENTPARTINDEX@" + actionParameters).c_str());
}
