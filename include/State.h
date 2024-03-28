#include <string>

#include <Sample.h>
#include <Part.h>

#ifndef STATESTRUCT_H
#define STATESTRUCT_H

// enum Instrument
// {
//     INTRUMENT_DRUMRACK1 = 0,
//     INTRUMENT_DRUMRACK2 = 1,
//     INTRUMENT_DRUMRACK3 = 2,
//     INTRUMENT_DRUMRACK4 = 3,
//     INTRUMENT_DRUMRACK5 = 4,
//     INTRUMENT_DRUMRACK6 = 5,
//     INTRUMENT_DRUMRACK7 = 6,
//     INTRUMENT_SLICER = 7,
//     INTRUMENT_SAMPLER = 8,
//     INTRUMENT_SYNTH = 9,
// };

struct State
{
    int currentSongIndex;
    char *songName;
    int songTempo;

    std::vector<Sample> samples;
    // drumRack
    int drumRackSampleFileRefIndex1;
    int drumRackSampleFileRefIndex2;
    int drumRackSampleFileRefIndex3;
    int drumRackSampleFileRefIndex4;
    int drumRackSampleFileRefIndex5;
    int drumRackSampleFileRefIndex6;
    int drumRackSampleFileRefIndex7;
    // slicer
    // int slicerSampleFileRefIndex;
    // sampler
    // int samplerSampleFileRefIndex;
    // synth

    int currentPartIndex;
    // Instrument currentPartInstrument;
    int currentPartInstrumentIndex;
    int currentStaveIndex;
    int currentOctaveIndex;
    // parts
    std::vector<Part> parts;

    int currentStepIndex;

    int16_t _masterBuffer[PLAY_WAV_WAV_BUFFER_SIZE];
    bool isPlaying;
};
#endif // STATESTRUCT_H
