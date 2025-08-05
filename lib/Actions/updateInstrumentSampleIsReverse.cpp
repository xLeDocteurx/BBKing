// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleIsReverse(State *statePointer, std::string actionParameters)
{
    statePointer->instruments[statePointer->currentPartInstrumentIndex].isReverse = std::stof(actionParameters);
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLEISREVERSE@" + actionParameters).c_str());
}
