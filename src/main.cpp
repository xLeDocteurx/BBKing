#include <stdio.h>
#include <vector>
#include <string>

#include <driver/gpio.h>
// #include <esp_spiffs.h>
// #include <driver/i2s.h>
// #include <driver/i2s_common.h>
// #include <driver/i2s_std.h>
// #include <driver/i2s_pdm.h>
// #include <driver/i2s_tdm.h>
#include "esp_http_server.h"

// TODO : Remove
#include "esp_dsp.h"

// libs

// Includes
#include <Defs.h>
// -----
#include <Init.h>
#include <SDCard.h>
#include <MyUtils.h>
#include <Effects.h>
#include <Tasks.h>
#include <Songs.h>

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

    vTaskDelay(pdMS_TO_TICKS(1500));

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
    // listFiles();
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
    initRes = initI2S(&state);
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

    printf("----- INIT TASKS -----\n");
    // xTaskCreatePinnedToCore(keyboardTask, "keyboardTask", 4096, &state, 10, &keyboardTaskHandle, 0);
    xTaskCreatePinnedToCore(sequencerTask, "sequencerTask", 4096, &state, 10, &sequencerTaskHandle, 0);
    xTaskCreatePinnedToCore(audioTask, "audioTask", 8192, &state, 10, &audioTaskHandle, 1);
    // // xTaskCreatePinnedToCore(keyboardTask, "keyboardTask", 4096, &state, 10, &keyboardTaskHandle, 1);
    // xTaskCreatePinnedToCore(sequencerTask, "sequencerTask", 4096, &state, 10, &sequencerTaskHandle, 1);
    // xTaskCreatePinnedToCore(audioTask, "audioTask", 8192, &state, 10, &audioTaskHandle, 0);
    printf("----- INIT TASKS DONE -----\n");

    // size_t signalLength = 4;
    // float signal[signalLength] = {1.0, 2.0, 3.0, 4.0};
    // size_t kernelLength = 3;
    // float kernel[kernelLength] = {5.0, 6.0, 7.0};
    // size_t outputLength = signalLength + kernelLength - 1;
    // float output[outputLength] = {};
    // dsps_conv_f32_ae32(signal, signalLength, kernel, kernelLength, output);

    // std::string outputString = "";
    // for (size_t i = 0; i < outputLength; i++)
    // {
    //     outputString += std::to_string(output[i]);
    //     outputString += ", ";
    // }
    // printf("dsps_conv_f32_ae32 output : %s\n", outputString.c_str());

    // int16_t inputBuffer[3] = {1, 2, 3};
    // int16_t memoryBufferChunk[3] = {0};
    // int16_t memoryBuffer[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    // printf("----------\n");
    // for (size_t i = 0; i < (sizeof(inputBuffer) / sizeof(inputBuffer[0])); i++)
    // {
    //     printf(",%i", inputBuffer[i]);
    // }
    // printf("---\n");
    // for (size_t i = 0; i < (sizeof(memoryBufferChunk) / sizeof(memoryBufferChunk[0])); i++)
    // {
    //     printf(",%i", memoryBufferChunk[i]);
    // }
    // printf("---\n");
    // for (size_t i = 0; i < (sizeof(memoryBuffer) / sizeof(memoryBuffer[0])); i++)
    // {
    //     printf(",%i", memoryBuffer[i]);
    // }
    // printf("---\n");

    // memcpy(memoryBufferChunk, memoryBuffer, 3 * sizeof(int16_t));
    // int startIndex = 0;
    // int endIndex = 3 - 1;
    // int numOfElements = 10 - (endIndex - startIndex + 1);
    // memmove(memoryBuffer + startIndex, memoryBuffer + endIndex + 1, numOfElements * sizeof(int16_t));
    // int arraysDiff = 10 - 3;
    // memmove(memoryBuffer + arraysDiff, inputBuffer, 3 * sizeof(int16_t));
    // memcpy(inputBuffer, memoryBufferChunk, 3 * sizeof(int16_t));

    // printf("----------\n");
    // for (size_t i = 0; i < (sizeof(inputBuffer) / sizeof(inputBuffer[0])); i++)
    // {
    //     printf(",%i", inputBuffer[i]);
    // }
    // printf("---\n");
    // for (size_t i = 0; i < (sizeof(memoryBufferChunk) / sizeof(memoryBufferChunk[0])); i++)
    // {
    //     printf(",%i", memoryBufferChunk[i]);
    // }
    // printf("---\n");
    // for (size_t i = 0; i < (sizeof(memoryBuffer) / sizeof(memoryBuffer[0])); i++)
    // {
    //     printf(",%i", memoryBuffer[i]);
    // }
    // printf("---\n");

    // printf("----------\n");

    printf("!!!!! TOUT MARCHE BIEN NAVETTE !!!!!\n");

    // state.songTempo = 55;
    // state.currentPartIndex = 2;
    // state.isPlaying = true;
    // state.masterGain = 0.5;

    // while (true)
    // {
    //     vTaskDelay(pdMS_TO_TICKS(10000));
    //     state.isBlbl = !state.isBlbl;
    // }
}
