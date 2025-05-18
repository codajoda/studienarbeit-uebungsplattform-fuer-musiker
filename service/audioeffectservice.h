#ifndef AUDIOEFFECTSERVICE_H
#define AUDIOEFFECTSERVICE_H

#include "EffectAudioSource.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>

class AudioEffectService
{
public:
    AudioEffectService(juce::AudioDeviceManager& sharedAudioDeviceManager);
    ~AudioEffectService();

    void addEffect(AudioEffectType type);
    void removeEffect(int effectIndex);
    void moveEffectFromTo(int fromIndex, int toIndex);
    void clearEffects();

    juce::AudioSource *getOutputSource();
    QStringList getUsingEffects();

private:
    juce::AudioDeviceManager& audioDeviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;
    EffectAudioSource* outputSource;
    std::vector<EffectAudioSource*> effects;
};

#endif // AUDIOEFFECTSERVICE_H
