#include "waveformservice.h"
#include <QtCore/qdebug.h>

std::vector<std::pair<double, float>> WaveformService::extractSamplesWithTimestamps(const juce::File& file, int downsampleFactor, int startTime, int duration)
{
    std::vector<std::pair<double, float>> waveformData;

    formatManager.clearFormats();
    formatManager.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader == nullptr)
    {
        DBG("Error: Could not load file.");
        return waveformData;
    }
    int numChannels = reader->numChannels;
    double sampleRate = reader->sampleRate;
    int numSamples = static_cast<int>(reader->lengthInSamples);

    if (numSamples < downsampleFactor)
        downsampleFactor = 1;

    juce::AudioBuffer<float> buffer(numChannels, numSamples);
    reader->read(&buffer, 0, numSamples, 0, true, true);

    for (int i = 0; i < numSamples; i += downsampleFactor)
    {
        float sampleSum = 0.0f;
        int count = 0;

        for (int j = 0; j < downsampleFactor && (i + j) < numSamples; ++j)
        {
            for (int channel = 0; channel < numChannels; ++channel)
            {
                sampleSum += buffer.getSample(channel, i + j);
            }
            count += numChannels;
        }

        float averagedSample = (count > 0) ? (sampleSum / count) : 0.0f;
        double timestamp = (startTime / 1000.0) + (static_cast<double>(i + downsampleFactor / 2) / sampleRate);

        waveformData.emplace_back(timestamp, averagedSample);
    }
    waveformData.emplace_back(duration / 1000, 0);
    return waveformData;
}
