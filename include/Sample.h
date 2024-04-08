#include <string>

#ifndef SAMPLESTRUCT_H
#define SAMPLESTRUCT_H
struct Sample
{
    char *filePath;
    bool isMono;
    float volume;
    int pitch;

    size_t fileSize;
    int16_t *buffer;    
    bool isPlaying;
    int bufferSamplesReadCounter;
    int startingStepVolume;
    int startingStepPitch;
};
#endif // SAMPLESTRUCT_H
