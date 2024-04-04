#include <State.h>

#ifndef GETCURRENTMODEASSTRING_H
#define GETCURRENTMODEASSTRING_H

std::string getCurrentModeAsString(State *statePointer)
{
    std::string ret = "";
    switch (statePointer->currentModeIndex)
    {
    case 0:
        ret.append("Part");
        break;
    case 1:
        ret.append("Step");
        break;
    }
    return ret;
}
#endif // GETCURRENTMODEASSTRING_H