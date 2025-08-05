// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSamplePitch(State *statePointer, std::string actionParameters)
{
    statePointer->instruments[statePointer->currentPartInstrumentIndex].pitch = stoi(actionParameters);
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLEPITCH@" + actionParameters).c_str());
}
