#include <vector>
#include <string>

#include <cJSON.h>

#include <Defs.h>
#include <Songs.h>
#include <MyUtils.h>
#include <SDCard.h>

bool saveSong(State *statePointer)
{
    std::string jsonString;
    bool readJsonFileRet = readJsonFile("/sdcard/songs.json", &jsonString);
    if (!readJsonFileRet)
    {
        printf("Failed to readJsonFile \"/sdcard/songs.json\"\n");
        return false;
    }

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

    // TODO : error handling;
    cJSON *songFromState = cJSON_CreateObject();
    // TODO : error handling
    getMachineStateAsCJson(statePointer, songFromState);

    cJSON_ReplaceItemInArray(root, statePointer->currentSongIndex, songFromState);

    // printf("savesong  root->valuestring : %s\n", root->valuestring);

    // TODO : error handling
    // writeFile("/data/songs.json", root->valuestring);
    writeFile("/sdcard/songs.json", cJSON_Print(root));

    return true;
}
