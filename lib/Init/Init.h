#include <stdio.h>
#include <esp_http_server.h>
#include <esp_wifi.h>

#include <driver/i2s_common.h>
#include <driver/i2s_std.h>
#include <driver/i2s_pdm.h>
#include <driver/i2s_tdm.h>

#include <Defs.h>

#ifndef INIT_H
#define INIT_H

bool initPSRAM();
bool initFileSystem();
bool initState(State *statePointer);
bool initI2S(State *statePointer);
// bool initI2S(i2s_chan_handle_t *tx_handle, i2s_chan_handle_t *rx_handle);
bool initKeyboard(); 
bool initWebServer(State *statePointer_p, httpd_handle_t *serverPointer_p, esp_netif_t *netif);

esp_netif_t *initWifi(std::vector<wifi_config_t> *wifiConfigsPointerParameter);

#endif // INIT_H