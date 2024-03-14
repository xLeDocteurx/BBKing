#include <stdio.h>
#include <vector>
#include <string>

#include <driver/gpio.h>
#include <esp_spiffs.h>
#include <driver/i2s.h>
#include "esp_http_server.h"

// libs

// Includes
#include <GlobalVars.h>
#include <WifiConfig.h>
#include <Sample.h>
#include <State.h>
// -----
#include <initPSRAM.h>
#include <initFileSystem.h>
#include <initWifi.h>
#include <initWebServer.h>
#include <initState.h>
#include <initI2S.h>
#include <initKeyboard.h>
#include <sequencerTask.h>
#include <audioTask.h>
#include <keyboardTask.h>

std::vector<wifi_config_t> wifiConfigs = {};
esp_netif_t *sta_netif;
httpd_handle_t httpServer = NULL;
// std::vector<Sample> samples = {};
State state;

TaskHandle_t testTaskHandle;
void testTask(void *parameter)
{
    State *statePointer = (State *)parameter;

    for (int i = 0; i < statePointer->samples.size(); i++)
    {
        // printf("WTF statePointer->samples[i].filePath : %s\n", statePointer->samples[i].filePath);
        // play_wav_file(statePointer->samples[i].filePath);

        // printf("0\n");
        // const char *filename = (char *)parameter;
        printf("play_wav_file(%s);\n", statePointer->samples[i].filePath);
        // i2s_dac_init();

        // printf("1\n");
        FILE *file = fopen(statePointer->samples[i].filePath, "rb");
        if (file == NULL)
        {
            // ESP_LOGE("play_wav_file", "Failed to open file");
            printf("Failed to open file\n");
            return;
        }
        // printf("2\n");

        // rewind(file);
        fseek(file, 44, SEEK_SET); // Skip WAV file header (44 bytes)
        // printf("3\n");

        // const int buffer_size = 512;
        // const int buffer_size = 256;
        int16_t buffer[PLAY_WAV_WAV_BUFFER_SIZE];
        // printf("4\n");

        while (1)
        {
            // printf("4.51\n");
            size_t bytes_read = fread(buffer, sizeof(int16_t) / 2, PLAY_WAV_WAV_BUFFER_SIZE, file);
            if (bytes_read <= 0)
            {
                printf("stop(%s);\n", statePointer->samples[i].filePath);
                break; // End of file or error
            }
            size_t bytes_written; // Initialize bytes_written variable
            // printf("4.52\n");
            // i2s_write(I2S_NUM_0, buffer, bytes_read, NULL, portMAX_DELAY);

            i2s_write(I2S_NUM_0, buffer, bytes_read, &bytes_written, portMAX_DELAY);
            // writeToMasterBuffer(buffer);

            // printf("4.53\n");
        }
        // printf("5\n");

        fclose(file);
    }

    // xTaskCreatePinnedToCore(sequencerTask, "sequencerTask", 8192, statePointer, 10, &sequencerTaskHandle, 0);
    // xTaskCreatePinnedToCore(audioTask, "audioTask", 8192, statePointer, 10, &audioTaskHandle, 1);

    vTaskDelay(pdMS_TO_TICKS(2500));

    statePointer->isPlaying = true;

    vTaskDelete(testTaskHandle);
}

extern "C" void app_main()
{
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    // gpio_set_level(LED_PIN, 0);

    vTaskDelay(pdMS_TO_TICKS(1000));
    bool initRes = true;
    printf("----- INIT PSRAM -----\n");
    initRes = initPSRAM();
    if (!initRes)
    {
        printf("----- INIT PSRAM FAILED -----\n");
        return;
    }
    printf("----- INIT PSRAM DONE -----\n");
    printf("----- INIT PSRAM -----\n");
    initRes = initFileSystem();
    if (!initRes)
    {
        printf("----- INIT FILE SYSTEM FAILED -----\n");
        return;
    }
    printf("----- INIT FILE SYSTEM DONE -----\n");
    printf("----- INIT WIFI -----\n");
    // initRes = initWifi(&wifiConfigs, sta_netif);
    esp_netif_t *initWifiRes = initWifi(&wifiConfigs);
    if (initWifiRes == NULL)
    {
        printf("----- INIT WIFI FAILED -----\n");
        return;
    }
    else
    {
        sta_netif = initWifiRes;
        printf("----- INIT WIFI DONE -----\n");
        // vTaskDelay(pdMS_TO_TICKS(10000));
        printf("iwd sta_netif int : %p\n", (void *)sta_netif);
        printf("iwd sta_netif int : %p\n", sta_netif);
        // vTaskDelay(pdMS_TO_TICKS(5000));
        printf("----- INIT WEB SERVER -----\n");
        initRes = initWebServer(&state, &httpServer, sta_netif);
        if (!initRes)
        {
            printf("----- INIT WEB SERVER FAILED -----\n");
            return;
        }
        printf("----- INIT WEB SERVER DONE -----\n");
    }
    // TODO : Uncomment
    printf("----- INIT STATE -----\n");
    initRes = initState(&state);
    if (!initRes)
    {
        printf("----- INIT STATE FAILED -----\n");
        return;
    }
    printf("----- INIT STATE DONE -----\n");
    printf("----- INIT I2S -----\n");
    initRes = initI2S();
    if (!initRes)
    {
        printf("----- INIT I2S FAILED -----\n");
        return;
    }
    printf("----- INIT I2S DONE -----\n");
    // printf("----- INIT KEYBOARD -----\n");
    // initRes = initKeyboard();
    // if (!initRes)
    // {
    //     printf("----- INIT KEYBOARD FAILED -----\n");
    //     return;
    // }
    // printf("----- INIT KEYBOARD DONE -----\n");
    printf("----- INIT TASKS -----\n");
    // xTaskCreatePinnedToCore(testTask, "testTask", 4096, &state, 10, &testTaskHandle, 0);
    // vTaskDelay(pdMS_TO_TICKS(7500));
    xTaskCreatePinnedToCore(sequencerTask, "sequencerTask", 4096, &state, 10, &sequencerTaskHandle, 0);
    xTaskCreatePinnedToCore(audioTask, "audioTask", 8192, &state, 10, &audioTaskHandle, 1);
    // xTaskCreatePinnedToCore(keyboardTask, "keyboardTask", 4096, &state, 10, &keyboardTaskHandle, 0);
    printf("----- INIT TASKS DONE -----\n");

    printf("!!!!! TOUT MARCHE BIEN NAVETTE !!!!!\n");

    // printf("----- SONG DEBUGING -----\n");
    // printf("state.songIndex : %i\n", state.currentPartIndex);
    // printf("state.songName : %s\n", state.songName);
    // printf("state.songTempo : %i\n", state.songTempo);
    // // printf("state.songStaves : %i\n", state.songStaves);
    // // printf("state.songBeats : %i\n", state.songBeats);
    // // printf("state.songMeasures : %i\n", state.songMeasures);
    // printf("----- SONG DEBUGING DONE -----\n");

    // vTaskDelay(pdMS_TO_TICKS(1000));

    // state.isPlaying = true;

    // // Turn off the LED
    // gpio_set_level(LED_PIN, 1);
}
