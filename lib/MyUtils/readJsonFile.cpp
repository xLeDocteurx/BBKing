#include <MyUtils.h>
#include <string>

#include <esp_spiffs.h>
#include <cJSON.h>
#include <cJSON_Utils.h>

#include <Defs.h>

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
