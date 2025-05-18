#include "audiomonitoringservice.h"
#include "audiocontroller.h"
#include <QDebug>

AudioMonitoringService::AudioMonitoringService(juce::AudioSource& inputSource, juce::AudioDeviceManager& sharedAudioDeviceManager, AudioController &audioController)
    : inputSource(inputSource), audioDeviceManager(sharedAudioDeviceManager), audioController(audioController)
{
    running = false;
    audioDeviceManager.addAudioCallback(this);
    gain = 1.0f;
}

AudioMonitoringService::~AudioMonitoringService()
{
    audioDeviceManager.removeAudioCallback(this);
}

void AudioMonitoringService::start()
{
    if (!running)
    {
        qDebug() << "Starte Audio-Capturing...";
        running = true;
    }
}

void AudioMonitoringService::stop()
{
    if (running)
    {
        qDebug() << "Stoppe Audio-Capturing...";
        running = false;
    }
}

bool AudioMonitoringService::isRunning() const
{
    return running;
}

void AudioMonitoringService::audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                                         int totalNumInputChannels,
                                                         float* const* outputChannelData,
                                                         int totalNumOutputChannels,
                                                         int numSamples,
                                                         [[maybe_unused]] const juce::AudioIODeviceCallbackContext& context)
{
    if (!running || totalNumOutputChannels == 0) {
        for (int chan = 0; chan < totalNumOutputChannels; ++chan)
        {
            auto* out = outputChannelData[chan];
            for (int i = 0; i < numSamples; ++i)
                out[i] = 0.0f;
        }
        return;
    }

    float* combinedChannel = new float[numSamples];

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float mixedSample = 0.0f;

        for (int inputChannel = 0; inputChannel < totalNumInputChannels; ++inputChannel)
        {
            if (inputChannelData[inputChannel] != nullptr)
            {
                mixedSample += inputChannelData[inputChannel][sample];
            }
        }
        combinedChannel[sample] = mixedSample;
    }
    juce::AudioBuffer<float> buffer(&combinedChannel, 1, numSamples);
    juce::AudioSourceChannelInfo info(&buffer, 0, numSamples);
    this->inputSource.getNextAudioBlock(info);

    for (int outputChan = 0; outputChan < totalNumOutputChannels; ++outputChan)
    {
        auto* outBuffer = outputChannelData[outputChan];

        for (int sample = 0; sample < numSamples; ++sample)
        {
            outBuffer[sample] = combinedChannel[sample] * 2.0f * gain;
        }
    }
    delete[] combinedChannel;
}
void AudioMonitoringService::audioDeviceAboutToStart(juce::AudioIODevice* device)
{
    if (device == nullptr)
    {
        qDebug() << "Kein aktives Audio-Gerät!";
        return;
    }

    qDebug() << "Neues Audio-Gerät gestartet: " << device->getName().toUTF8();
}

void AudioMonitoringService::audioDeviceStopped()
{
    qDebug() << "Altes Audio-Gerät gestoppt";
}

void AudioMonitoringService::setInputVolume(float gain)
{
    this->gain = gain;
}

