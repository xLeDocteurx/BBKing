#include <SDCard.h>

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"

bool listFiles()
{
    printf("Listing files in /sdcard:\n");

    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir("/sdcard")) == NULL)
    {
        printf("Failed to open directory for reading\n");
        return false;
    }

    // telldir(dir);

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
        // printf("/sdcard/%s\n", entry->d_name);
        // printf("%d/%i : %s\n", entry->d_ino, entry->d_type, "/sdcard" + "/" + entry->d_name);
    }

    closedir(dir);

    return true;
}