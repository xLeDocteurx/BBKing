#include <SDCard.h>

#include <stdio.h>
#include <string.h>
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"

bool readJsonFile(const char *path, std::string *jsonStringPointer)
{
    // Open JSON file for reading
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Failed to open file \"%s\" for reading\n", path);
        return false;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate memory to store the JSON contents
    char* jsonBuffer = (char*)malloc(fileSize + 1);
    if (jsonBuffer == NULL) {
        printf("Failed to allocate memory for JSON buffer\n");
        fclose(file);
        return false;
    }

    // Read JSON contents into the buffer
    fread(jsonBuffer, 1, fileSize, file);
    fclose(file);
    jsonBuffer[fileSize] = '\0'; // Null-terminate the string
    

    jsonStringPointer->append(jsonBuffer);
    free(jsonBuffer);

    return true;
}