#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleEndPosition(State *statePointer, std::string actionParameters)
{
    if (statePointer->instruments[statePointer->currentPartInstrumentIndex].get()->type == DRUM_RACK)
    {
        static_cast<DrumRack *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->endPosition = std::stof(actionParameters);
        broadcast_ws_message(("UPDATEINSTRUMENTSAMPLEENDPOSITION@" + actionParameters).c_str());
    }
}
