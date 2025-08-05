// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void selectOctave(State *statePointer, std::string actionParameters)
{
    int desiredOctaveIndex = stoi(actionParameters);
    // if (desiredOctaveIndex >= 0 && desiredOctaveIndex <= 8)
    // {
    statePointer->currentOctaveIndex = desiredOctaveIndex;
    // }
    broadcast_ws_message(("SELECTOCTAVE@" + actionParameters).c_str());
}
