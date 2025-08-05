// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>
#include <MyUtils.h>

void updateSelectedInstrumentSample(State *statePointer, std::string actionParameters)
{
    int desiredSampleFileIndex = stoi(actionParameters);
    // Clear memory from previous sample
    freeFile(statePointer->instruments[statePointer->currentPartInstrumentIndex].buffer);

    // TODO : Error handling
    // bool loadInstrumentRes =
    loadInstrument(statePointer->wavFilePaths[desiredSampleFileIndex], true, statePointer->instruments[statePointer->currentPartInstrumentIndex].volume, statePointer->instruments[statePointer->currentPartInstrumentIndex].pitch, statePointer->instruments[statePointer->currentPartInstrumentIndex].startPosition, statePointer->instruments[statePointer->currentPartInstrumentIndex].endPosition, statePointer->instruments[statePointer->currentPartInstrumentIndex].isReverse, &statePointer->instruments[statePointer->currentPartInstrumentIndex]);
    // if (!loadInstrumentRes)
    // {
    //     printf("Failed to update file from %s to %s\n", statePointer->instruments[statePointer->currentPartInstrumentIndex].sample.filePath, statePointer->wavFilePaths[desiredSampleFileIndex]);
    // }
    broadcast_ws_message(("UPDATESELECTEDINSTRUMENTSAMPLE@" + actionParameters).c_str());
}
