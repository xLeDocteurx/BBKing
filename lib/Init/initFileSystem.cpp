#include <Init.h>
#include <esp_spiffs.h>
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"

bool initFileSystem()
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/data",
        .partition_label = "data",
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

    printf("1\n");
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        // .max_files = 10
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};
    sdmmc_card_t *card;
    printf("2\n");
    sdmmc_host_t host_config = SDMMC_HOST_DEFAULT();

/**
 * On these chips, the SDMMC IO power is supplied externally
 */
#if CONFIG_EXAMPLE_SDMMC_IO_POWER_INTERNAL_LDO
    sd_pwr_ctrl_ldo_config_t ldo_config = {
        .ldo_chan_id = 4, // `LDO_VO4` is used as the SDMMC IO power
    };
    sd_pwr_ctrl_handle_t pwr_ctrl_handle = NULL;

    ret = sd_pwr_ctrl_new_on_chip_ldo(&ldo_config, &pwr_ctrl_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to new an on-chip ldo power control driver");
        return;
    }
    host.pwr_ctrl_handle = pwr_ctrl_handle;
#endif

    printf("3\n");
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

// Set bus width to use:
#ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
    slot_config.width = 4;
#else
    slot_config.width = 1;
#endif

    slot_config.cmd = SD_MMC_CMD;
    slot_config.clk = SD_MMC_CLK;
    slot_config.d0 = SD_MMC_D0;

    // Enable internal pullups on enabled pins. The internal pullups
    // are insufficient however, please make sure 10k external pullups are
    // connected on the bus. This is for debug / example purpose only.
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    printf("4\n");

    vTaskDelay(pdMS_TO_TICKS(1500));

    ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host_config, &slot_config, &mount_config, &card);
    printf("5\n");
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            printf("Failed to mount filesystem. Error: %s\n", esp_err_to_name(ret));
        }
        else
        {
            printf("Failed to initialize the card (%s).\n", esp_err_to_name(ret));
            // check_sd_card_pins(&config, pin_count);
        }
        return false;
    }

    return true;
}
