// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void play(State *statePointer, std::string actionParameters)
{
    statePointer->isPlaying = true;
    broadcast_ws_message(("PLAY@" + actionParameters).c_str());
}
