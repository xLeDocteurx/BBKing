#include <stdio.h>
#include <esp_http_server.h>
#include <esp_wifi.h>

#include <Defs.h>

#ifndef INIT_H
#define INIT_H

bool initFileSystem();
bool initI2S();
bool initKeyboard();
bool initPSRAM();
bool initState(State *statePointer);
bool initWebServer(State *statePointer_p, httpd_handle_t *serverPointer_p, esp_netif_t *netif);
esp_netif_t *initWifi(std::vector<wifi_config_t> *wifiConfigsPointerParameter);

#endif // INIT_H