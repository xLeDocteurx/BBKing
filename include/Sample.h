#include <string>

#ifndef SAMPLESTRUCT_H
#define SAMPLESTRUCT_H
struct Sample
{
    // TODO : Remove filePath
    char *filePath;
    FILE *file;
    bool isPlaying;
    int bufferBytesReadCounter;
};
#endif // SAMPLESTRUCT_H
