#ifndef AUDIOMANAGERSERVICE_H
#define AUDIOMANAGERSERVICE_H

#include "juce_audio_devices/juce_audio_devices.h"
#include <QtCore/qcontainerfwd.h>
#include <QList>
#include <QtCore/qdebug.h>

class AudioManagerService
{
public:
    explicit AudioManagerService(juce::AudioDeviceManager& sharedAudioDeviceManager);
    ~AudioManagerService();

    QStringList getAvailableInputDevices();
    QStringList getAvailableInputChannels();
    QStringList getAvailableOutputDevices();
    QStringList getAvailableOutputChannels();
    QStringList getAvailableSampleRates();
    QStringList getAvailableBufferSizes();

    QString getCurrentInputDevice();
    QStringList getCurrentInputChannels();
    QString getCurrentOutputDevice();
    QStringList getCurrentOutputChannels();
    QString getCurrentSampleRate();
    QString getCurrentBufferSize();


    void setInputDevice(const QString& deviceName);
    void setInputChannel(int channel, bool active);
    void setOutputDevice(const QString& deviceName);
    void setOutputChannel(int channel, bool active);
    void setSampleRate(QString sampleRate);
    void setBufferSize(QString bufferSize);

private:
    juce::AudioDeviceManager& audioDeviceManager;
};

#endif // AUDIOMANAGERSERVICE_H
