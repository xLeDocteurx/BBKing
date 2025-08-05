// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleVolume(State *statePointer, std::string actionParameters)
{
    statePointer->instruments[statePointer->currentPartInstrumentIndex].volume = std::stof(actionParameters);
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLEVOLUME@" + actionParameters).c_str());
}
