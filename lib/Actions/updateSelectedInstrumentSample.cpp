// #include <string>
#include <stdio.h>

#include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>
#include <MyUtils.h>

void updateSelectedInstrumentSample(State *statePointer, std::string actionParameters)
{
    if (statePointer->instruments[statePointer->currentPartInstrumentIndex].get()->type == DRUM_RACK || statePointer->instruments[statePointer->currentPartInstrumentIndex].get()->type == SAMPLER)
    {
        // Clear memory from previous sample
        free(statePointer->instruments[statePointer->currentPartInstrumentIndex].get()->buffer);

        // TODO : Error handling ?
        if (statePointer->instruments[statePointer->currentPartInstrumentIndex]->type == DRUM_RACK)
        {
            loadDrumRack(
                static_cast<DrumRack *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get()),
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->isSolo,
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->isMuted,
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->volume,
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->pitch,
                statePointer->wavFilePaths[stoi(actionParameters)],
                static_cast<DrumRack *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->isReverse,
                static_cast<DrumRack *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->startPosition,
                static_cast<DrumRack *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->endPosition);
        }
        else if (statePointer->instruments[statePointer->currentPartInstrumentIndex]->type == SAMPLER)
        {
            loadSampler(
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get()),
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->isSolo,
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->isMuted,
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->volume,
                statePointer->instruments[statePointer->currentPartInstrumentIndex]->pitch,
                statePointer->wavFilePaths[stoi(actionParameters)],
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->isLooping,
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->loopStartPosition,
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->loopEndPosition,
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->attackPosition,
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->decayPosition,
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->sustainPosition,
                static_cast<Sampler *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->releasePosition);
        }

        broadcast_ws_message(("UPDATESELECTEDINSTRUMENTSAMPLE@" + actionParameters).c_str());
    }
}
