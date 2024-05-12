#include <vector>
#include <string>

#include <cJSON.h>

#include <Defs.h>
#include <Songs.h>
#include <MyUtils.h>
#include <SDCard.h>

bool saveSong(State *statePointer)
{
    printf("1\n");
    std::string jsonString;
    bool readJsonFileRet = readJsonFile("/data/songs.json", &jsonString);
    if (!readJsonFileRet)
    {
        printf("Failed to readJsonFile \"/data/songs.json\"\n");
        return false;
    }
    printf("2\n");

    // Parse JSON
    cJSON *root = cJSON_Parse(jsonString.c_str());
    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error before: %s\n", error_ptr);
        }
        printf("Failed to parse JSON\n");
        // free(jsonBuffer);
        return NULL;
    }
    printf("3\n");

    // TODO : error handling;
    cJSON *songFromState = cJSON_CreateObject();
    printf("4\n");
    // TODO : error handling
    getMachineStateAsCJson(statePointer, songFromState);
    printf("5\n");

    cJSON_ReplaceItemInArray(root, statePointer->currentSongIndex, songFromState);
    printf("6\n");


    // TODO : error handling
    // writeFile("/data/songs.json", root->valuestring);
    writeFile("/data/songs.json", cJSON_Print(root));
    printf("7\n");

    return true;
}
