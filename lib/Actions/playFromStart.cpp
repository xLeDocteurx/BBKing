// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void playFromStart(State *statePointer, std::string actionParameters)
{
    statePointer->currentStepIndex = 0;
    statePointer->isPlaying = true;
    broadcast_ws_message(("PLAYFROMSTART@" + actionParameters).c_str());
}
