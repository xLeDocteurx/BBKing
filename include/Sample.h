#include <string>

#ifndef SAMPLESTRUCT_H
#define SAMPLESTRUCT_H
struct Sample
{
    char *filePath;
    size_t fileSize;
    int16_t *buffer;    
    bool isPlaying;
    int bufferSamplesReadCounter;
};
#endif // SAMPLESTRUCT_H
