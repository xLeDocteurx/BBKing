#include <stdio.h>
#include <esp_http_server.h>
#include <esp_wifi.h>

#include <Defs.h>

#ifndef SDCARD_H
#define SDCARD_H

bool listFiles();
bool readFile(const char* path);
bool readJsonFile(const char* path, std::string *jsonStringPointer);
bool writeFile(const char* path);

#endif // SDCARD_H