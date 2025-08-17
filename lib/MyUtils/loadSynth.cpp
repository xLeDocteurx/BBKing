#include <vector>
#include <string>

#include <esp_psram.h>
#include <esp_system.h>
// #include <driver/spi_master.h>

#include <Defs.h>
// #include <MyUtils.h>

bool loadSynth(
    Synth *instrumentPointer,
    bool isSolo, bool isMuted, float volume, int pitch,
    WaveFormType osc1WaveFormType, WaveFormType osc2WaveFormType, WaveFormType osc3WaveFormType)
{
    // int16_t *fileBufferPointer = (int16_t *)malloc(sample.fileSize);
    int16_t *fileBufferPointer = nullptr;

    // TODO : Load waveforms from files ?
    // Or from memory ?
    *instrumentPointer = {SYNTH, isSolo, isMuted, false, volume, 0, pitch, 0, fileBufferPointer, 0, osc1WaveFormType, osc2WaveFormType, osc3WaveFormType};
    return true;
}
