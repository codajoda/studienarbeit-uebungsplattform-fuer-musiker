#ifndef AUDIOMONITORINGSERVICE_H
#define AUDIOMONITORINGSERVICE_H

#include "juce_audio_devices/juce_audio_devices.h"
#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>

class AudioController;

class AudioMonitoringService : public juce::AudioSourcePlayer
{
public:
    explicit AudioMonitoringService(juce::AudioSource& inputSource, juce::AudioDeviceManager& sharedAudioDeviceManager, AudioController& audioController);
    ~AudioMonitoringService() override;

    void start();
    void stop();
    bool isRunning() const;

    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                          int numInputChannels,
                                          float* const* outputChannelData,
                                          int numOutputChannels,
                                          int numSamples,
                                          const juce::AudioIODeviceCallbackContext& context) override;

    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;

    void setInputVolume(float gain);

private:
    juce::AudioDeviceManager& audioDeviceManager;
    juce::AudioBuffer<float> buffer;
    AudioController& audioController;
    bool running;

    float gain;

    juce::AudioSource& inputSource;
};

#endif // AUDIOMONITORINGSERVICE_H
