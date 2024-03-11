#include <string>

#include <esp_spiffs.h>
#include <cJSON.h>
#include <cJSON_Utils.h>

#include <GlobalVars.h>

#ifndef READJSONFILE_H
#define READJSONFILE_H

// TODO : Remove sizeToRead and calculate file size to make this variable
bool readJsonFile(char *filePath, std::string *stringPointer)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        // ESP_LOGE("play_wav_file", "Failed to open file");
        printf("Failed to open file : %s\n", filePath);
        return false;
    }

    // fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)

    // int buffer[JSON_BUFFER_SIZE];

    // while (1)
    // {
    // size_t bytes_read = fread(buffer, sizeof(int), JSON_BUFFER_SIZE, file);
    // size_t bytes_read = fread(charsReference, sizeof(int), JSON_BUFFER_SIZE, file);
    // if (bytes_read <= 0)
    // {
    //     printf("end of file;\n");
    //     // break; // End of file or error
    // }
    // else
    // {
    //     printf("bytes_read : %i\n", bytes_read);
    // }

    // Read and print each character until the end of file
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        // printf("%c", (char)c);
        stringPointer->push_back(c);
    }
    // printf("\n");

    // for (int i = 0; i < JSON_BUFFER_SIZE; i++)
    // {
    //     // printf("bi : %i\n", *charsReference[i]);
    //     // printf("bf : %f\n", *charsReference[i]);
    //     // printf("bd : %d\n", *charsReference[i]);
    //     // printf("bs : %s\n", *charsReference[i]);
    // }
    // }

    fclose(file);
    return true;
}

#endif // READJSONFILE_H
