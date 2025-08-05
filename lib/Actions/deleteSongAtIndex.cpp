// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void deleteSongAtIndex(State *statePointer, std::string actionParameters)
{
    int songIndex = stoi(actionParameters);
    deleteSongAtIndex(statePointer, songIndex);
    statePointer->currentSongIndex = songIndex > 0 ? songIndex - 1 : 0;
    readSong(statePointer, songIndex);
    broadcast_ws_message(("DELETESONGATINDEX@" + actionParameters).c_str());
}
