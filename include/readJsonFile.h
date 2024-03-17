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
        printf("Failed to open file : %s\n", filePath);
        return false;
    }

    int c;
    while ((c = fgetc(file)) != EOF)
    {
        // printf("%c", (char)c);
        stringPointer->push_back(c);
    }

    fclose(file);
    return true;
}

#endif // READJSONFILE_H
