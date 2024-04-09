#include <GlobalVars.h>

#include <State.h>

#ifndef EFFECTMASTERDISTORTION_H
#define EFFECTMASTERDISTORTION_H

void masterEffectDistortion(State* statePointer)
{
    for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++) {
        statePointer->_masterBuffer[i] = statePointer->_masterBuffer[i];
    }
}

#endif // EFFECTMASTERDISTORTION_H
