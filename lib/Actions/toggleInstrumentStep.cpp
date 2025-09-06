// #include <string>
#include <stdio.h>

// #include <Defs.h>
#include <Actions.h>
#include <Init.h>
#include <Songs.h>

void toggleInstrumentStep(State *statePointer, std::string actionParameters)
{
    const int stepIndex = stoi(actionParameters);

    bool isDrumRackSampleStepActive = false;
    for (int stepContentIndex = 0; stepContentIndex < statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].size(); stepContentIndex++)
    {
        if (statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].get()->instrumentIndex == statePointer->currentPartInstrumentIndex)
        {
            std::vector<std::unique_ptr<Step>>::iterator it = statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].begin() + stepContentIndex;
            if (it != statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].end())
            {
                isDrumRackSampleStepActive = true;
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].erase(it);
            }
            break;
        }

        if (!isDrumRackSampleStepActive)
        {

            if (statePointer->instruments[statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].get()->instrumentIndex].get()->type == DRUM_RACK_STEP)
            {
                auto step = std::make_unique<DrumRackStep>(statePointer->currentPartInstrumentIndex, 1.0, 0, false, 0.0, 1.0);
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back(std::move(step));
            }
            else if (statePointer->instruments[statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].get()->instrumentIndex].get()->type == SAMPLER_STEP)
            {
                auto step = std::make_unique<SamplerStep>(statePointer->currentPartInstrumentIndex, 1.0, 0);
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back(std::move(step));
            }
            else if (statePointer->instruments[statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].get()->instrumentIndex].get()->type == SYNTH_STEP)
            {
                auto step = std::make_unique<SynthStep>(statePointer->currentPartInstrumentIndex, 1.0, 0);
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back(std::move(step));
            }
        }
        broadcast_ws_message(("TOGGLEINSTRUMENTSTEP@" + actionParameters).c_str());
    }
}
