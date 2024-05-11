#include <stdio.h>
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <cJSON.h>

#include <Defs.h>

#ifndef MYUTILS_H
#define MYUTILS_H

std::string getCurrentModeAsString(State *statePointer);
void getMachineStateAsCJson(State *statePointer, cJSON *cjsonStringPointer);
void getWavFilesAsJsonString(State *statePointer, std::string *jsonStringPointer);
bool getIndexHtml(std::string *stringPointer);

void freeFile(void *filePointer);
// TODO : Rename as "loadSampleIntoInstrument"
bool loadInstrument(char *filePath, bool isMono, float volume, int pitch, float startPosition, float endPosition, bool isReverse, Instrument *instrumentPointer);

float pitchToPlaybackSpeed(int pitch);

#endif // MYUTILS_H