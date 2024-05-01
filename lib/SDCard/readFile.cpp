#include <SDCard.h>

#include <stdio.h>
#include <string.h>
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"

bool readFile(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Failed to open file for reading\n");
        return false;
    }

    char line[128];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    fclose(file);

    return true;
}