// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateSongName(State *statePointer, std::string actionParameters)
{
    strcpy(statePointer->songName, actionParameters.c_str());
    broadcast_ws_message(("UPDATESONGNAME@" + actionParameters).c_str());
}
