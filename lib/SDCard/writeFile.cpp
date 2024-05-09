#include <SDCard.h>

#include <stdio.h>
#include <string.h>
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"

bool writeFile(const char *path)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("Failed to create file\n");
        return false;
    }

    fprintf(file, "Hello world\n");
    fclose(file);

    return true;
}