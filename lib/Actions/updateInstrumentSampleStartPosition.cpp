#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleStartPosition(State *statePointer, std::string actionParameters)
{
    if (statePointer->instruments[statePointer->currentPartInstrumentIndex].get()->type == DRUM_RACK)
    {
        static_cast<DrumRack *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->startPosition = std::stof(actionParameters);
        broadcast_ws_message(("UPDATEINSTRUMENTSAMPLESTARTPOSITION@" + actionParameters).c_str());
    }
}
