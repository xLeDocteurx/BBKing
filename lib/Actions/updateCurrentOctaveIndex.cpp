// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>

void updateCurrentOctaveIndex(State *statePointer, std::string actionParameters)
{
    statePointer->currentOctaveIndex = stoi(actionParameters);
    broadcast_ws_message(("UPDATECURRENTOCTAVEINDEX@" + actionParameters).c_str());
}
