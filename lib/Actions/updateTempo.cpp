// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateTempo(State *statePointer, std::string actionParameters)
{
    statePointer->songTempo = stoi(actionParameters);
    broadcast_ws_message(("UPDATETEMPO@" + actionParameters).c_str());
}
