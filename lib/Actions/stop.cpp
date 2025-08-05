// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void stop(State *statePointer, std::string actionParameters)
{
    statePointer->isPlaying = false;
    statePointer->currentStepIndex = 0;
    broadcast_ws_message(("STOP@" + actionParameters).c_str());
}
