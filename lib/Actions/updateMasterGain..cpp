// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>

void updateMasterGain(State *statePointer, std::string actionParameters)
{
    statePointer->masterGain = stof(actionParameters);
    broadcast_ws_message(("UPDATEMASTERGAIN@" + actionParameters).c_str());
}
