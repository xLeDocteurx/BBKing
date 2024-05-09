#include <MyUtils.h>

#include <math.h>

float pitchToPlaybackSpeed(int pitch) {
    return pow(2, static_cast<float>(pitch) / 12.0);
}
