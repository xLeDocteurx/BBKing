#include <Defs.h>

#ifndef SONGS_H
#define SONGS_H

bool createSongAtIndex(State *statePointer, int index);
bool readSong(State *statePointer, int songIndex);
bool saveSong(State *statePointer);
bool deleteSongAtIndex(State *statePointer, int index);

#endif // SONGS_H
