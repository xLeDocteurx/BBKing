#include <SDCard.h>

#include <stdio.h>
#include <string.h>
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"

bool writeFile(const char *path, char *content)
{
    printf("6.1\n");
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("Failed to create file\n");
        return false;
    }
    printf("6.2\n");

    fprintf(file, content);
    printf("6.3\n");
    fclose(file);
    printf("6.4\n");

    return true;
}