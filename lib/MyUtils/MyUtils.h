#include <stdio.h>
#include <esp_http_server.h>
#include <esp_wifi.h>

#include <Defs.h>

#ifndef MYUTILS_H
#define MYUTILS_H

std::string getCurrentModeAsString(State *statePointer);
void getMachineStateAsJsonString(State *statePointer, std::string *jsonStringPointer);
bool readJsonFile(char *filePath, std::string *stringPointer);

void freeFile(void *filePointer);
bool loadInstrument(char *filePath, bool isMono, float volume, int pitch, Instrument *instrumentPointer);

float pitchToPlaybackSpeed(int pitch);

#endif // MYUTILS_H