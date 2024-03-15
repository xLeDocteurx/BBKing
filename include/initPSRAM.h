#include <esp_psram.h>

#include <GlobalVars.h>

bool initPSRAM()
{
    esp_err_t ret_init_psram = esp_psram_init();

    if (ret_init_psram != ESP_OK)
    {
        printf("Failed to run esp_psram_init\n");
        return false;
    }

    // size_t psramSize = esp_psram_get_size();
    // printf("PSRAM size : %i\n", psramSize);

    return true;
}
