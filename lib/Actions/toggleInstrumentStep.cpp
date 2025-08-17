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
        if (statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].instrumentIndex == statePointer->currentPartInstrumentIndex)
        {
            std::vector<Step>::iterator it = statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].begin() + stepContentIndex;
            if (it != statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].end())
            {
                isDrumRackSampleStepActive = true;
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].erase(it);
            }
            break;
        }

        if (!isDrumRackSampleStepActive)
        {

            if (statePointer->instruments[statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].instrumentIndex].get()->type == DRUM_RACK_STEP)
            {
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back({statePointer->currentPartInstrumentIndex, 1.0, 0, false, 0.0, 1.0});
            }
            else if (statePointer->instruments[statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].instrumentIndex].get()->type == SAMPLER_STEP)
            {
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back({statePointer->currentPartInstrumentIndex, 1.0, 0});
            }
            else if (statePointer->instruments[statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].instrumentIndex].get()->type == SYNTH_STEP)
            {
                statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back({
                    statePointer->currentPartInstrumentIndex,
                    1.0,
                    0,
                });
            }
        }
        broadcast_ws_message(("TOGGLEINSTRUMENTSTEP@" + actionParameters).c_str());
    }
}
