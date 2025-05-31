#include <MyUtils.h>
#include <SDCard.h>
#include <string>

#include <Defs.h>

// TODO : Redo with cJson
void getWavFilesAsJsonString(State *statePointer, std::string *jsonStringPointer)
{
    std::string wavFilePathsString = "[";
    for (int i = 0; i < statePointer->wavFilePaths.size(); i++)
    {
        if (i != 0)
        {
            wavFilePathsString += ",";
        }
        wavFilePathsString += "\"";
        wavFilePathsString += statePointer->wavFilePaths[i];
        wavFilePathsString += "\"";
    }
    wavFilePathsString += "]";

    // std::string jsonString =
    //     "{\"wavFilePaths\":" +
    //     wavFilePathsString +
    //     "}";

    // jsonStringPointer->append(jsonString);
    jsonStringPointer->append(wavFilePathsString);
}
