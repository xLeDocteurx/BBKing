#include <stdio.h>
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <cJSON.h>

#include <Defs.h>

#ifndef MYUTILS_H
#define MYUTILS_H

  
// // One function works for all data types. 
// // This would work even for user defined types 
// // if operator '>' is overloaded 
// template <typename T>

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}
uint32_t clampInt32(uint32_t input, uint32_t min, uint32_t max);

std::string getCurrentModeAsString(State *statePointer);
void getMachineStateAsCJson(State *statePointer, cJSON *cjsonStringPointer);
void getNewSongAsCJson(State *statePointer, cJSON *cjsonObjectPointer, int newSongIndex);
void getWavFilesAsJsonString(State *statePointer, std::string *jsonStringPointer);
bool getIndexHtml(std::string *stringPointer);

void freeFile(void *filePointer);
// TODO : Rename as "loadSampleIntoInstrument"
bool loadInstrument(char *filePath, bool isMono, float volume, int pitch, float startPosition, float endPosition, bool isReverse, Instrument *instrumentPointer);

float pitchToPlaybackSpeed(int pitch);

#endif // MYUTILS_H