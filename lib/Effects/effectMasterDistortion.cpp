#include <Defs.h>

void masterEffectDistortion(State* statePointer)
{
    for (int i = 0; i < PLAY_WAV_WAV_BUFFER_SIZE; i++) {
        statePointer->_masterBuffer[i] = statePointer->_masterBuffer[i];
    }
}
