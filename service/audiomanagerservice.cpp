#include "audiomanagerservice.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include <QString>
#include <QStringList>
#include <QtCore/qdebug.h>

AudioManagerService::AudioManagerService(juce::AudioDeviceManager &sharedAudioDeviceManager)
    : audioDeviceManager(sharedAudioDeviceManager) {}
AudioManagerService::~AudioManagerService()
{
    audioDeviceManager.closeAudioDevice();
}

QStringList AudioManagerService::getAvailableInputDevices()
{
    QStringList devices;
    const juce::OwnedArray<juce::AudioIODeviceType>& deviceTypes = audioDeviceManager.getAvailableDeviceTypes();
    for (int i = 0; i < deviceTypes.size(); ++i)
    {
        juce::AudioIODeviceType* deviceType = deviceTypes[i];
        if (deviceType != nullptr)
        {
            juce::StringArray inputDevices = deviceType->getDeviceNames(true);
            for (int j = 0; j < inputDevices.size(); ++j)
            {
                juce::String inputDevice = inputDevices[j];
                if (inputDevice.isNotEmpty())
                    devices.append(QString::fromStdString(inputDevice.toStdString()));
            }
        }
    }
    return devices;
}

QStringList AudioManagerService::getAvailableInputChannels()
{
    QStringList inputChannels;
    auto* device = audioDeviceManager.getCurrentAudioDevice();
    if (device) {
        int numInputChannels = device->getInputChannelNames().size();
        for (int i = 0; i < numInputChannels; ++i) {
            juce::String channelName = device->getInputChannelNames()[i];
            inputChannels.append(QString::fromStdString(channelName.toStdString()));
        }
    }
    return inputChannels;
}

QStringList AudioManagerService::getAvailableOutputDevices()
{
    QStringList devices;
    const juce::OwnedArray<juce::AudioIODeviceType>& deviceTypes = audioDeviceManager.getAvailableDeviceTypes();
    for (int i = 0; i < deviceTypes.size(); ++i)
    {
        juce::AudioIODeviceType* deviceType = deviceTypes[i];
        if (deviceType != nullptr)
        {
            juce::StringArray outputDevices = deviceType->getDeviceNames(false);
            for (int j = 0; j < outputDevices.size(); ++j)
            {
                juce::String outputDevice = outputDevices[j];
                if (outputDevice.isNotEmpty())
                    devices.append(QString::fromStdString(outputDevice.toStdString()));
            }
        }
    }
    return devices;
}

QStringList AudioManagerService::getAvailableOutputChannels()
{
    QStringList outputChannels;
    auto* device = audioDeviceManager.getCurrentAudioDevice();

    if (device) {
        int numOutputChannels = device->getOutputChannelNames().size();
        for (int i = 0; i < numOutputChannels; ++i) {
            juce::String channelName = device->getOutputChannelNames()[i];
            outputChannels.append(QString::fromStdString(channelName.toStdString()));
        }
    }
    return outputChannels;
}

QStringList AudioManagerService::getAvailableSampleRates()
{
    QStringList sampleRates;
    auto* device = audioDeviceManager.getCurrentAudioDevice();
    if (device)
    {
        for (double rate : device->getAvailableSampleRates())
            sampleRates.append(QString::number(rate));
    }
    return sampleRates;
}

QStringList AudioManagerService::getAvailableBufferSizes()
{
    QStringList bufferSizes;
    auto* device = audioDeviceManager.getCurrentAudioDevice();

    if (device)
    {
        for (int size : device->getAvailableBufferSizes())
            bufferSizes.append(QString::number(size));
    }
    return bufferSizes;
}

QString AudioManagerService::getCurrentInputDevice()
{
    auto setup = audioDeviceManager.getAudioDeviceSetup();
    return QString::fromStdString(setup.inputDeviceName.toStdString());
}

QStringList AudioManagerService::getCurrentInputChannels()
{
    QStringList currentInputChannels;
    auto* device = audioDeviceManager.getCurrentAudioDevice();
    if (device) {
        juce::StringArray activeInputChannels = device->getInputChannelNames();
        for (int i = 0; i < activeInputChannels.size(); ++i) {
            currentInputChannels.append(QString::fromStdString(activeInputChannels[i].toStdString()));
        }
    }
    return currentInputChannels;
}

QString AudioManagerService::getCurrentOutputDevice()
{
    auto setup = audioDeviceManager.getAudioDeviceSetup();
    return QString::fromStdString(setup.outputDeviceName.toStdString());
}

QStringList AudioManagerService::getCurrentOutputChannels()
{
    QStringList currentOutputChannels;
    auto* device = audioDeviceManager.getCurrentAudioDevice();
    if (device) {
        juce::StringArray activeOutputChannels = device->getOutputChannelNames();

        for (int i = 0; i < activeOutputChannels.size(); ++i) {
            currentOutputChannels.append(QString::fromStdString(activeOutputChannels[i].toStdString()));
        }
    }
    return currentOutputChannels;
}

QString AudioManagerService::getCurrentSampleRate()
{
    auto* device = audioDeviceManager.getCurrentAudioDevice();
    return device ? QString::number(device->getCurrentSampleRate()) : "Unknown";
}

QString AudioManagerService::getCurrentBufferSize()
{
    auto* device = audioDeviceManager.getCurrentAudioDevice();
    return device ? QString::number(device->getCurrentBufferSizeSamples()) : "Unknown";
}


void AudioManagerService::setInputDevice(const QString& deviceName)
{
    juce::AudioDeviceManager::AudioDeviceSetup setup = audioDeviceManager.getAudioDeviceSetup();
    setup.inputDeviceName = deviceName.toStdString();
    audioDeviceManager.setAudioDeviceSetup(setup, true);
}

void AudioManagerService::setOutputDevice(const QString& deviceName)
{
    juce::AudioDeviceManager::AudioDeviceSetup setup = audioDeviceManager.getAudioDeviceSetup();
    setup.outputDeviceName = deviceName.toStdString();
    audioDeviceManager.setAudioDeviceSetup(setup, true);
}

void AudioManagerService::setSampleRate(QString sampleRate)
{
    juce::AudioDeviceManager::AudioDeviceSetup setup = audioDeviceManager.getAudioDeviceSetup();
    setup.sampleRate = sampleRate.toDouble();
    audioDeviceManager.setAudioDeviceSetup(setup, true);
}

void AudioManagerService::setBufferSize(QString bufferSize)
{
    juce::AudioDeviceManager::AudioDeviceSetup setup = audioDeviceManager.getAudioDeviceSetup();
    setup.bufferSize = bufferSize.toInt();
    audioDeviceManager.setAudioDeviceSetup(setup, true);
}

void AudioManagerService::setInputChannel(int channel, bool active)
{
    juce::AudioDeviceManager::AudioDeviceSetup setup = audioDeviceManager.getAudioDeviceSetup();
    if (!setup.useDefaultInputChannels)
    {
        if (active) {
            setup.inputChannels.setBit(channel);
        } else {
            setup.inputChannels.clearBit(channel);
        }
    }
    else
    {
        setup.useDefaultInputChannels = false;
        if (active) {
            setup.inputChannels.setBit(channel);
        } else {
            setup.inputChannels.clearBit(channel);
        }
    }
    audioDeviceManager.setAudioDeviceSetup(setup, true);
}

void AudioManagerService::setOutputChannel(int channel, bool active)
{
    juce::AudioDeviceManager::AudioDeviceSetup setup = audioDeviceManager.getAudioDeviceSetup();
    if (!setup.useDefaultOutputChannels)
    {
        if (active) {
            setup.outputChannels.setBit(channel);
        } else {
            setup.outputChannels.clearBit(channel);
        }
    }
    else
    {
        setup.useDefaultOutputChannels = false;
        if (active) {
            setup.outputChannels.setBit(channel);
        } else {
            setup.outputChannels.clearBit(channel);
        }
    }
    audioDeviceManager.setAudioDeviceSetup(setup, true);
}
