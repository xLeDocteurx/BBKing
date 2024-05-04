#include <MyUtils.h>
#include <string>

#include <esp_spiffs.h>
#include <cJSON.h>
#include <cJSON_Utils.h>

#include <Defs.h>

// TODO : Remove sizeToRead and calculate file size to make this variable
// TODO : Rename as getIndexHtml
bool getIndexHtml(std::string *stringPointer)
{
    FILE *file = fopen("/data/index.html", "r");
    if (file == NULL)
    {
        printf("Failed to open file : %s\n", "/data/index.html");
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
