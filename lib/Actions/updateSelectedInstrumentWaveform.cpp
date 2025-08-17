// #include <string>
#include <stdio.h>

#include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>
#include <MyUtils.h>

// TODO
// void updateSelectedInstrumentWaveform(State *statePointer, std::string actionParameters)
// {
//     if (statePointer->instruments[statePointer->currentPartInstrumentIndex].get()->type == SYNTH)
//     {
//         // Clear memory from previous sample
//         free(statePointer->instruments[statePointer->currentPartInstrumentIndex].get()->buffer);

//         loadSynth(
//             static_cast<Synth *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get()),
//             statePointer->instruments[statePointer->currentPartInstrumentIndex]->isSolo,
//             statePointer->instruments[statePointer->currentPartInstrumentIndex]->isMuted,
//             statePointer->instruments[statePointer->currentPartInstrumentIndex]->volume,
//             statePointer->instruments[statePointer->currentPartInstrumentIndex]->pitch,
//             static_cast<Synth *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->osc1WaveFormType,
//             static_cast<Synth *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->osc2WaveFormType,
//             static_cast<Synth *>(statePointer->instruments[statePointer->currentPartInstrumentIndex].get())->osc3WaveFormType);

//         broadcast_ws_message(("UPDATESELECTEDINSTRUMENTWAVEFORM@" + actionParameters).c_str());
//     }
// }
