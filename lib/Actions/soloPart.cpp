// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void soloPart(State *statePointer, std::string actionParameters)
{
    // TODO : Implement SOLO PART
    int desiredIndex = stoi(actionParameters);
    // for (int i = 0; i < statePointer->instruments.size(); i++)
    // {
    //     // if(statePointer->instruments[i] == i) {
    //     //     // TODO
    //     //     // statePointer->samples[statePointer->instruments[i].sampleFileRefIndex].
    //     // } else {

    //     // }
    // }
    broadcast_ws_message(("SOLOPART@" + actionParameters).c_str());
}
