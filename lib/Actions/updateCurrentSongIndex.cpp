// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateCurrentSongIndex(State *statePointer, std::string actionParameters)
{
    int songIndex = stoi(actionParameters);
    readSong(statePointer, songIndex);
    broadcast_ws_message(("UPDATECURRENTSONGINDEX@" + actionParameters).c_str());
}
