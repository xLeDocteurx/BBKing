#include <Init.h>
#include <esp_psram.h>

#include <Defs.h>

bool initPSRAM()
{
    if(esp_psram_is_initialized())
    {
        printf("PSRAM already init\n");
    }
    else
    {
        esp_err_t ret_init_psram = esp_psram_init();
        if (ret_init_psram != ESP_OK)
        {
            printf("Failed to run esp_psram_init\n");
            return false;
        }
    }

    size_t psramSize = esp_psram_get_size();
    printf("PSRAM size : %i\n", psramSize);

    size_t freePsramSize = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    printf("Free PSRAM size : %i\n", freePsramSize);
    size_t freeInternalRamSize = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    printf("Free Internal RAM size : %i\n", freeInternalRamSize);

    return true;
}
