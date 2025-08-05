// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void createSongAtIndex(State *statePointer, std::string actionParameters)
{
    int songIndex = stoi(actionParameters);
    createSongAtIndex(statePointer, songIndex);
    statePointer->currentSongIndex = songIndex;
    readSong(statePointer, songIndex);
    broadcast_ws_message(("CREATESONGATINDEX@" + actionParameters).c_str());
}
