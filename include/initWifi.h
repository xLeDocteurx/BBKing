#include <vector>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
// #include "tcpip_adapter.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
// #include "esp_http_server.h"

#include <readJsonFile.h>
#include <WifiConfig.h>

// esp_netif_t *sta_netif;

// TODO : Event handler
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        printf("!!! Event !!! Wi-Fi connected\n");
        // vTaskDelay(pdMS_TO_TICKS(5000));
        // printf("----- INIT WEB SERVER -----\n");
        // bool initRes = initWebServer(&httpServer, sta_netif);
        // if (!initRes)
        // {
        //     printf("----- INIT WEB SERVER FAILED -----\n");
        //     // return;
        // }
        // printf("----- INIT WEB SERVER DONE -----\n");

        // Turn off the LED
        gpio_set_level(LED_PIN, 1);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        printf("!!! Event !!! Wi-Fi disconnected\n");
    }
}

// bool initWifi(std::vector<wifi_config_t> *wifiConfigsPointer, esp_netif_t *netif)
esp_netif_t *initWifi(std::vector<wifi_config_t> *wifiConfigsPointer)
{
    gpio_set_level(LED_PIN, 0);
    // esp_netif_t *netif_v = netif;
    // char charss[JSON_BUFFER_SIZE];
    std::string jsonString;
    bool readJsonFile_ret = readJsonFile("/data/wifiConfigs.json", &jsonString);
    if (!readJsonFile_ret)
    {
        printf("Failed to read wifiConfigs.json file\n");
        return NULL;
    }
    else
    {
        printf("initWifi readJsonFile wifiConfigs.json : %s\n", jsonString.c_str());
    }

    wifiConfigsPointer->push_back({
        .sta = {
            .ssid = "SFR_25AF",
            .password = "pk88u26tgpg1k48xrue2",
            .pmf_cfg = {
                .capable = true,
                .required = false,
            },
            // .failure_retry_cnt = 5,
            .failure_retry_cnt = 10,
        },
    });
    wifiConfigsPointer->push_back({"Tardigrad", "blblblbl"});
    wifiConfigsPointer->push_back({
        .sta = {
            .ssid = "Tardigrad",
            .password = "blblblbl",
            .pmf_cfg = {
                .capable = true,
                .required = false,
            },
            // .failure_retry_cnt = 5,
            .failure_retry_cnt = 10,
        },
    });
    wifiConfigsPointer->push_back({"Backstage-WiFi", "44VWSvwMpXXqy3b0GXedTi5R1LtGVnYS"});
    wifiConfigsPointer->push_back({
        .sta = {
            .ssid = "Backstage-WiFi",
            .password = "44VWSvwMpXXqy3b0GXedTi5R1LtGVnYS",
            .pmf_cfg = {
                .capable = true,
                .required = false,
            },
            // .failure_retry_cnt = 5,
            .failure_retry_cnt = 10,
        },
    });

    esp_err_t nvs_flash_init_ret = nvs_flash_init();
    if (nvs_flash_init_ret != ESP_OK)
    {
        printf("Failed to init NVS flash\n");
        return NULL;
    }

    // tcpip_adapter_init();
    esp_err_t esp_netif_init_ret = esp_netif_init();
    if (esp_netif_init_ret != ESP_OK)
    {
        printf("Failed to esp_netif_init\n");
        return NULL;
    }
    esp_err_t esp_event_loop_create_default_ret = esp_event_loop_create_default(); // Create the default event loop
    if (esp_event_loop_create_default_ret != ESP_OK)
    {
        printf("Failed to esp_event_loop_create_default\n");
        return NULL;
    }

    // Create the Wi-Fi station interface
    esp_netif_t *netif_ret = esp_netif_create_default_wifi_sta();

    printf("initWifi netif_ret int : %p\n", (void *)netif_ret);
    printf("initWifi netif_ret int : %p\n", netif_ret);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t esp_wifi_init_ret = esp_wifi_init(&cfg);
    // ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    if (esp_wifi_init_ret != ESP_OK)
    {
        printf("Failed to init wifi\n");
        return NULL;
    }

    // TODO : Event handler
    // Register event handlers
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);

    // Set the default Wi-Fi event handler
    esp_wifi_set_default_wifi_sta_handlers();

    // ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    esp_err_t esp_wifi_set_mode_ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (esp_wifi_set_mode_ret != ESP_OK)
    {
        printf("Failed to set wifi mode\n");
    }

    bool hasEstablishedConnection = false;
    for (int i = 0; i < wifiConfigsPointer->size(); i++)
    {
        if (!hasEstablishedConnection)
        {
            printf("Trying to connect to %s\n", wifiConfigsPointer->at(i).sta.ssid);

            // ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
            esp_err_t esp_wifi_set_config_ret = esp_wifi_set_config(WIFI_IF_STA, &wifiConfigsPointer->at(i));
            if (esp_wifi_set_config_ret != ESP_OK)
            {
                printf("Failed to set wifi config\n");
            }
            else
            {
                // printf("4\n");
                // ESP_ERROR_CHECK(esp_wifi_start());
                esp_err_t esp_wifi_start_ret = esp_wifi_start();
                if (esp_wifi_start_ret != ESP_OK)
                {
                    printf("Failed to start wifi\n");
                }
                else
                {

                    // This API only impact WIFI_MODE_STA or WIFI_MODE_APSTA mode
                    // If station interface is connected to an AP, call esp_wifi_disconnect to disconnect.
                    // The scanning triggered by esp_wifi_scan_start() will not be effective until connection between device and the AP is established. If device is scanning and connecting at the same time, it will abort scanning and return a warning message and error number ESP_ERR_WIFI_STATE.
                    // This API attempts to connect to an Access Point (AP) only once. To enable reconnection in case of a connection failure, please use the 'failure_retry_cnt' feature in the 'wifi_sta_config_t'. Users are suggested to implement reconnection logic in their application for scenarios where the specified AP does not exist, or reconnection is desired after the device has received a disconnect event.

                    // Connect to the Wi-Fi network (configure SSID and password before calling this function)
                    esp_err_t esp_wifi_connect_ret = esp_wifi_connect();
                    if (esp_wifi_connect_ret != ESP_OK)
                    {
                        printf("Failed to connect to wifi : %s\n", wifiConfigsPointer->at(i).sta.ssid);
                    }
                    else
                    {
                        printf("Connected to wifi : %s\n", wifiConfigsPointer->at(i).sta.ssid);
                        hasEstablishedConnection = true;
                    }
                    // printf("esp_wifi_start_ret : %i\n", esp_wifi_start_ret);
                    // printf("5\n");
                    // esp_wifi_
                }
            }
        }
    }
    // return hasEstablishedConnection;
    return netif_ret;
}
