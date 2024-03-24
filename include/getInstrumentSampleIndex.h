#include <State.h>

int getInstrumentSampleIndex(State *statePointer, int instrumentIndex)
{
    int instrumentSampleIndex = 0;
    switch (instrumentIndex)
    {
    case 0:
        instrumentSampleIndex = statePointer->drumRackSampleFileRefIndex1;
        break;
    case 1:
        instrumentSampleIndex = statePointer->drumRackSampleFileRefIndex2;
        break;
    case 2:
        instrumentSampleIndex = statePointer->drumRackSampleFileRefIndex3;
        break;
    case 3:
        instrumentSampleIndex = statePointer->drumRackSampleFileRefIndex4;
        break;
    case 4:
        instrumentSampleIndex = statePointer->drumRackSampleFileRefIndex5;
        break;
    case 5:
        instrumentSampleIndex = statePointer->drumRackSampleFileRefIndex6;
        break;
    case 6:
        instrumentSampleIndex = statePointer->drumRackSampleFileRefIndex7;
        break;
    }

    return instrumentSampleIndex;
}