// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>

void updateCurrentMode(State *statePointer, std::string actionParameters)
{
    statePointer->currentModeIndex = stoi(actionParameters);
    broadcast_ws_message(("UPDATECURRENTMODE@" + actionParameters).c_str());
}
