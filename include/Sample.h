#include <string>

#ifndef SAMPLESTRUCT_H
#define SAMPLESTRUCT_H
struct Sample
{
    // TODO : Remove filePath
    char *filePath;
    // FILE *file;
    size_t fileSize;
    // TODO : malloc pointer for free()
    int16_t *buffer;    
    bool isPlaying;
    int bufferBytesReadCounter;
};
#endif // SAMPLESTRUCT_H
