// #include <string>
#include <stdio.h>

#include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>
#include <MyUtils.h>

void updateSelectedInstrumentSample(State *statePointer, std::string actionParameters)
{
    int desiredSampleFileIndex = stoi(actionParameters);
    // Clear memory from previous sample
    freeFile(statePointer->instruments[statePointer->currentPartInstrumentIndex].buffer);

    // TODO : Error handling ?
    if (statePointer->instruments[statePointer->currentPartInstrumentIndex]->type == DRUM_RACK)
    {
        loadDrumRack();
    }
    else if (statePointer->instruments[statePointer->currentPartInstrumentIndex]->type == SAMPLER)
    {
        loadSampler();
    }
    else if (statePointer->instruments[statePointer->currentPartInstrumentIndex]->type == SYNTH)
    {
        loadSynth();
    }

    broadcast_ws_message(("UPDATESELECTEDINSTRUMENTSAMPLE@" + actionParameters).c_str());
}
