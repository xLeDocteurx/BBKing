#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleEndPosition(State *statePointer, std::string actionParameters)
{
    statePointer->instruments[statePointer->currentPartInstrumentIndex].endPosition = std::stof(actionParameters);
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLEENDPOSITION@" + actionParameters).c_str());
}
