// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void mutePart(State *statePointer, std::string actionParameters)
{
    // TODO : Implement MUTE PART
    int desiredIndex = stoi(actionParameters);
    broadcast_ws_message(("MUTEPART@" + actionParameters).c_str());
}
