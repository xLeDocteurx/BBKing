// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>

void updateCurrentSelectedStep(State *statePointer, std::string actionParameters)
{
    statePointer->currentSelectedStepIndex = stoi(actionParameters);
    broadcast_ws_message(("UPDATECURRENTSELECTEDSTEP@" + actionParameters).c_str());
}
