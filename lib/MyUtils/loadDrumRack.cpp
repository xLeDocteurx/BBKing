#include <vector>
#include <string>

#include <esp_psram.h>
#include <esp_system.h>
// #include <driver/spi_master.h>

#include <Defs.h>
// #include <MyUtils.h>

bool loadDrumRack(
    DrumRack *instrumentPointer,
    bool isSolo, bool isMuted, float volume, int pitch, const char *filePath,
    bool isReverse, float startPosition, float endPosition)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        printf("Failed to open file : %s\n", filePath);
        return false;
    }

    // Read the WAV header
    WavHeader header;
    fread(reinterpret_cast<char *>(&header), sizeof(uint8_t), sizeof(WavHeader), file);

    // Check if the file is a WAV file
    if (std::string(header.chunkID, 4) != "RIFF" || std::string(header.format, 4) != "WAVE")
    {
        printf("Not a WAV file! %s\n", filePath);
        return false;
    }

    // TODO : file size reading header data
    // size_t fileSize = ftell(file) - 44;
    size_t fileSize = header.subchunk2Size;
    printf("fileSize : %i\n", fileSize);

    // TODO : isMono from header
    Sample sample = {filePath, true, fileSize};

    fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)
    int16_t *fileBufferPointer = (int16_t *)malloc(sample.fileSize);

    // size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t), sample.fileSize / sizeof(int16_t), file);
    fread(fileBufferPointer, sizeof(int16_t), sample.fileSize / sizeof(int16_t), file);

    fclose(file);

    *instrumentPointer = {DRUM_RACK, isSolo, isMuted, false, volume, 0, pitch, 0, fileBufferPointer, 0, sample, isReverse, 0, startPosition, 0, endPosition, 1};
    return true;
}
