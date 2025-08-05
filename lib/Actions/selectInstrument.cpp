// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void selectInstrument(State *statePointer, std::string actionParameters)
{
    int desiredIntrumentIndex = stoi(actionParameters);
    // if (desiredIntrumentIndex >=0 && < 7)
    // {
    statePointer->currentPartInstrumentIndex = desiredIntrumentIndex;
    // }
    broadcast_ws_message(("SELECTINSTRUMENT@" + actionParameters).c_str());
}
