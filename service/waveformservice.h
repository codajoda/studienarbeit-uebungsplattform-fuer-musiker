#ifndef WAVEFORMSERVICE_H
#define WAVEFORMSERVICE_H

#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"
#include <vector>
#include <utility>

class WaveformService
{
public:
    WaveformService() = default;
    ~WaveformService() = default;

    std::vector<std::pair<double, float>> extractSamplesWithTimestamps(const juce::File& file, int downsampleFactor, int startTime, int duration);

private:
    juce::AudioFormatManager formatManager;
};

#endif // WAVEFORMSERVICE_H
