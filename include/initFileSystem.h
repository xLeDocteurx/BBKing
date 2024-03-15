#include <esp_spiffs.h>

bool initFileSystem()
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/data",
        .partition_label = "data",
        // .partition_label = "NULL",
        // .max_files = 1,
        // .max_files = 7,
        .max_files = 10,
        .format_if_mount_failed = false
        // .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            printf("Failed to mount or format filesystem\n");
            return false;
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            printf("Failed to find SPIFFS partition\n");
            return false;
        }
        else
        {
            printf("Failed to initialize SPIFFS (%s)\n", esp_err_to_name(ret));
            return false;
        }
        printf("Failed SPIFFS (UNKNOWN REASON)\n");
        return false;
    }
    return true;
}
