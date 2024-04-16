#include <stdio.h>
#include <vector>
#include <string>

#include <driver/gpio.h>
#include <esp_spiffs.h>
#include <driver/i2s.h>
#include "esp_http_server.h"

// libs

// Includes
#include <Defs.h>
// -----
#include <Init.h>
#include <MyUtils.h>
#include <Tasks.h>

// TODO : WHAT THE FUCK
TaskHandle_t audioTaskHandle;
TaskHandle_t keyboardTaskHandle;
TaskHandle_t sequencerTaskHandle;

std::vector<wifi_config_t> wifiConfigs = {};
esp_netif_t *sta_netif;
httpd_handle_t httpServer = NULL;
// std::vector<Sample> samples = {};
State state;

extern "C" void app_main()
{
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    bool initRes = true;
    printf("----- INIT PSRAM -----\n");
    initRes = initPSRAM();
    if (!initRes)
    {
        printf("----- INIT PSRAM FAILED -----\n");
        return;
    }
    printf("----- INIT PSRAM DONE -----\n");
    printf("----- INIT FILE SYSTEM -----\n");
    initRes = initFileSystem();
    if (!initRes)
    {
        printf("----- INIT FILE SYSTEM FAILED -----\n");
        return;
    }
    printf("----- INIT FILE SYSTEM DONE -----\n");
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
    // xTaskCreatePinnedToCore(keyboardTask, "keyboardTask", 4096, &state, 10, &keyboardTaskHandle, 0);
    xTaskCreatePinnedToCore(sequencerTask, "sequencerTask", 4096, &state, 10, &sequencerTaskHandle, 0);
    xTaskCreatePinnedToCore(audioTask, "audioTask", 8192, &state, 10, &audioTaskHandle, 1);
    printf("----- INIT TASKS DONE -----\n");

    printf("!!!!! TOUT MARCHE BIEN NAVETTE !!!!!\n");


    printf("----- INIT WIFI -----\n");
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
        printf("iwd sta_netif int : %p\n", (void *)sta_netif);
        printf("iwd sta_netif int : %p\n", sta_netif);
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
