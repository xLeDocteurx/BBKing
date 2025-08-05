#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleStartPosition(State *statePointer, std::string actionParameters)
{
    statePointer->instruments[statePointer->currentPartInstrumentIndex].startPosition = std::stof(actionParameters);
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLESTARTPOSITION@" + actionParameters).c_str());
}
