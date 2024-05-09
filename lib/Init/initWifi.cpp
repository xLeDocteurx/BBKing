#include <Init.h>
#include <vector>
#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
// #include "tcpip_adapter.h"
#include "esp_wifi.h"
#include "esp_event.h"
// #include "esp_log.h"
// #include "esp_http_server.h"
#include <cJSON.h>

#include <Defs.h>
#include <MyUtils.h>
#include <SDCard.h>

// esp_netif_t *sta_netif;
std::vector<wifi_config_t> *wifiConfigsPointer;

bool hasEstablishedConnection = false;

// TODO : Event handler
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        printf("!!! Event !!! Wi-Fi connected\n");
        hasEstablishedConnection = true;
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        printf("!!! Event !!! Wi-Fi disconnected\n");
        // TODO : handle reconnection ? ( differentiate failed first connection and a regular disconnection )

        // printf("!!! Event !!! Reconnecting...\n");
        // bool reconnectRes = reconnect(wifiConfigsPointer);
        // if(reconnectRes) {
        //     printf("!!! Event !!! Reconnect SUCCEEDED\n");
        // }
        // else
        // {
        //     printf("!!! Event !!! Reconnect FAILED\n");
        // }
    }
}

// bool initWifi(std::vector<wifi_config_t> *wifiConfigsPointer, esp_netif_t *netif)
esp_netif_t *initWifi(std::vector<wifi_config_t> *wifiConfigsPointerParameter)
{
    wifiConfigsPointer = wifiConfigsPointerParameter;

    gpio_set_level(LED_PIN, 1);

    std::string jsonString;
    bool readJsonFileRet = readJsonFile("/sdcard/wifiConfigs.json", &jsonString);
    if (!readJsonFileRet)
    {
        printf("Failed to readJsonFile \"/sdcard/wifiConfigs.json\"\n");
        return NULL;
    }
    // printf("-\n");
    // printf("-\n");
    // printf("-\n");
    // printf("%s\n", jsonString.c_str());
    // printf("-\n");
    // printf("-\n");
    // printf("-\n");

    // Parse JSON
    cJSON *root = cJSON_Parse(jsonString.c_str());
    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error before: %s\n", error_ptr);
        }
        printf("Failed to parse JSON\n");
        // free(jsonBuffer);
        return NULL;
    }

    // Example: Access JSON data
    // cJSON* name = cJSON_GetObjectItemCaseSensitive(root, "0");
    int arraySize = cJSON_GetArraySize(root);
    // std::vector<wifi_config_t> wifiConfigs;
    for (int i = 0; i < arraySize; i++)
    {
        // TODO : pushback error handling;
        cJSON *arrayItem = cJSON_GetArrayItem(root, i);
        cJSON *ssidJson = cJSON_GetObjectItemCaseSensitive(arrayItem, "ssid");
        // if (cJSON_IsString(ssid) && (ssid->valuestring != NULL))
        // {
        //     printf("ssid : %s\n", ssid->valuestring);
        // }
        cJSON *passwordJson = cJSON_GetObjectItemCaseSensitive(arrayItem, "password");
        // if (cJSON_IsString(password) && (password->valuestring != NULL))
        // {
        //     printf("password : %s\n", password->valuestring);
        // }
        char *ssid = ssidJson->valuestring;
        char *password = passwordJson->valuestring;
        wifi_config_t wifi_config = {
            .sta = {
                // .ssid = ssid->valuestring,
                // .password = password->valuestring,
                .pmf_cfg = {
                    .capable = true,
                    .required = false,
                },
                // .failure_retry_cnt = 5,
                .failure_retry_cnt = 10,
            },
        };

        // // destination first, source second, max size of buffer last
        // strlcpy(wifi_config.sta.ssid, ssid, 32);
        // strlcpy(wifi_config.sta.password, password, 32);

        size_t ssid_length = strlen(ssid);
        memcpy(wifi_config.sta.ssid, ssid, ssid_length);
        size_t password_length = strlen(password);
        memcpy(wifi_config.sta.password, password, password_length);

        wifiConfigsPointerParameter->push_back(wifi_config);
    }

    // Clean up
    cJSON_Delete(root);
    // free(&jsonString);

    // wifi_config_t wifi_config_1 = {
    //     .sta = {
    //         .ssid = "SFR_25AF",
    //         .password = "pk88u26tgpg1k48xrue2",
    //         .pmf_cfg = {
    //             .capable = true,
    //             .required = false,
    //         },
    //         // .failure_retry_cnt = 5,
    //         .failure_retry_cnt = 10,
    //     },
    // };
    // wifi_config_t wifi_config_2 = {
    //     .sta = {
    //         .ssid = "Tardigrad",
    //         .password = "blblblbl",
    //         .pmf_cfg = {
    //             .capable = true,
    //             .required = false,
    //         },
    //         .failure_retry_cnt = 10,
    //     },
    // };
    // wifi_config_t wifi_config_3 = {
    //     .sta = {
    //         .ssid = "Backstage-WiFi",
    //         .password = "44VWSvwMpXXqy3b0GXedTi5R1LtGVnYS",
    //         .pmf_cfg = {
    //             .capable = true,
    //             .required = false,
    //         },
    //         .failure_retry_cnt = 10,
    //     },
    // };
    // wifiConfigsPointerParameter->push_back(wifi_config_1);
    // wifiConfigsPointerParameter->push_back(wifi_config_3);
    // wifiConfigsPointerParameter->push_back(wifi_config_2);

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

    for (int i = 0; i < wifiConfigsPointerParameter->size(); i++)
    {
        if (!hasEstablishedConnection)
        {
            int atemptsCounter = 0;
            while (atemptsCounter < 3)
            {
                printf("ATEMPT : %i\n", atemptsCounter + 1);
                printf("Trying to connect to %s\n", wifiConfigsPointerParameter->at(i).sta.ssid);

                // ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
                esp_err_t esp_wifi_set_config_ret = esp_wifi_set_config(WIFI_IF_STA, &wifiConfigsPointerParameter->at(i));
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
                            printf("Failed to connect to wifi : %s\n", wifiConfigsPointerParameter->at(i).sta.ssid);
                        }
                        else
                        {
                            printf("Connecting to wifi : %s\n", wifiConfigsPointerParameter->at(i).sta.ssid);
                            vTaskDelay(pdMS_TO_TICKS(3000));
                            gpio_set_level(LED_PIN, 0);
                            if (hasEstablishedConnection)
                            {
                                printf("Connected to wifi : %s\n", wifiConfigsPointerParameter->at(i).sta.ssid);
                                break;
                            }
                            else
                            {
                                printf("Failed to connect to wifi : %s\n", wifiConfigsPointerParameter->at(i).sta.ssid);
                                vTaskDelay(pdMS_TO_TICKS(1000));
                                gpio_set_level(LED_PIN, 1);
                            }

                            // hasEstablishedConnection = true;
                            // gpio_set_level(LED_PIN, 0);
                            // break;
                        }
                        // printf("esp_wifi_start_ret : %i\n", esp_wifi_start_ret);
                        // printf("5\n");
                        // esp_wifi_
                    }
                }
                atemptsCounter += 1;
            }
        }
        else
        {
            break;
        }
    }
    // return hasEstablishedConnection;
    return netif_ret;
}
