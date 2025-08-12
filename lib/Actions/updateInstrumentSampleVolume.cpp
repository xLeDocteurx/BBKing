// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void updateInstrumentSampleVolume(State *statePointer, std::string actionParameters)
{
    if (std::holds_alternative<DrumRack>(statePointer->instruments[statePointer->currentPartInstrumentIndex]) || std::holds_alternative<Sampler>(statePointer->instruments[statePointer->currentPartInstrumentIndex]) || std::holds_alternative<Synth>(statePointer->instruments[statePointer->currentPartInstrumentIndex]))
    {
        auto &instrument = std::get<Instrument>(statePointer->instruments[statePointer->currentPartInstrumentIndex]);
        instrument.volume = std::stof(actionParameters);
    }
    broadcast_ws_message(("UPDATEINSTRUMENTSAMPLEVOLUME@" + actionParameters).c_str());
}
