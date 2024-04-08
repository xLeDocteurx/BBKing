#include <State.h>

#ifndef GETINSTRUMENTSAMPLEINDEX_H
#define GETINSTRUMENTSAMPLEINDEX_H

int getInstrumentSampleIndex(State *statePointer, int instrumentIndex)
{
    int instrumentSampleIndex = 0;
    switch (instrumentIndex)
    {
    case 0:
        instrumentSampleIndex = statePointer->drumRack0SampleFileRefIndex;
        break;
    case 1:
        instrumentSampleIndex = statePointer->drumRack1SampleFileRefIndex;
        break;
    case 2:
        instrumentSampleIndex = statePointer->drumRack2SampleFileRefIndex;
        break;
    case 3:
        instrumentSampleIndex = statePointer->drumRack3SampleFileRefIndex;
        break;
    case 4:
        instrumentSampleIndex = statePointer->drumRack4SampleFileRefIndex;
        break;
    case 5:
        instrumentSampleIndex = statePointer->drumRack5SampleFileRefIndex;
        break;
    case 6:
        instrumentSampleIndex = statePointer->drumRack6SampleFileRefIndex;
        break;
    }

    return instrumentSampleIndex;
}
#endif // GETINSTRUMENTSAMPLEINDEX_H