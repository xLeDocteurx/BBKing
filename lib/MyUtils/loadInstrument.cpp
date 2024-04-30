#include <vector>
#include <string>

#include <esp_psram.h>
#include <esp_system.h>
#include <driver/spi_master.h>

#include <Defs.h>
// #include <MyUtils.h>

// TODO : wtf ?
void freeFile(void *filePointer)
{
    free(filePointer);
}

bool loadInstrument(char *filePath, bool isMono, float volume, int pitch, Instrument *instrumentPointer)
{
    // Sample sample;
    // loadSample(filePath, isMono, &sample);

    printf("loadSample(%s);\n", filePath);

    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", filePath);
        return false;
    }
    // fseek(file, 0, SEEK_END);

    // Read the WAV header
    WavHeader header;
    fread(reinterpret_cast<char *>(&header), sizeof(u_int8_t), sizeof(WavHeader), file);

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
    // fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)

    //     int16_t *fileBufferPointer = (int16_t *)malloc(fileSize);
    // , fileBufferPointer, false, 0, 0, 0

    // size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t), fileSize / sizeof(int16_t), file);

    fclose(file);

    // TODO : isMono from header
    Sample sample = {filePath, isMono, fileSize};

    // FILE *file = fopen(filePath, "rb");
    // if (file == NULL)
    // {
    //     printf("Failed to open file %s\n", filePath);
    //     return false;
    // }
    fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)
    int16_t *fileBufferPointer = (int16_t *)malloc(sample.fileSize);

    // size_t bytes_read = fread(fileBufferPointer, sizeof(int16_t), sample.fileSize / sizeof(int16_t), file);
    fread(fileBufferPointer, sizeof(int16_t), sample.fileSize / sizeof(int16_t), file);

    fclose(file);

    *instrumentPointer = {sample, false, false, volume, pitch, fileBufferPointer, false, 0, 0, 0};
    return true;
}
