#include "audioeffectservice.h"
#include "DelayAudioSource.h"
#include "DistortionAudioSource.h"
#include "RawAudioSource.h"
#include "ReverbAudioSource.h"
#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>

AudioEffectService::AudioEffectService(juce::AudioDeviceManager& sharedAudioDeviceManager) : audioDeviceManager(sharedAudioDeviceManager)
{
    outputSource = new RawAudioSource();
    effects = { outputSource };
}

AudioEffectService::~AudioEffectService() {
    for (auto* effect : effects) {
        delete effect;
    }
}

void AudioEffectService::addEffect(AudioEffectType type) {
    EffectAudioSource* newEffect = nullptr;
    switch(type) {
    case AudioEffectType::Delay:
        newEffect = new DelayAudioSource();
        break;
    case AudioEffectType::Distortion:
        newEffect = new DistortionAudioSource();
        break;
    case AudioEffectType::Raw:
        newEffect = new RawAudioSource();
        break;
    case AudioEffectType::Reverb:
        newEffect = new ReverbAudioSource();
        break;
    default:
        qDebug("No Effect Found");
    }
    if (newEffect != nullptr) {
        if (auto* reverbEffect = dynamic_cast<ReverbAudioSource*>(newEffect)) {
            reverbEffect->setSource(new RawAudioSource());
        } else {
            newEffect->setSource(new RawAudioSource());
        }
        effects.back()->setSource(newEffect);
        effects.push_back(newEffect);
    }
}

void AudioEffectService::removeEffect(int effectIndex) {
    if (effectIndex > 0 && effectIndex < effects.size()) {
        auto* effectToRemove = effects[effectIndex];

        if (effectIndex == effects.size() - 1) {
            effects[effectIndex - 1]->setSource(new RawAudioSource());
        } else {
            effects[effectIndex - 1]->setSource(effects[effectIndex + 1]);
        }

        effects.erase(effects.begin() + effectIndex);

        delete effectToRemove;
    }
}

void AudioEffectService::moveEffectFromTo(int fromIndex, int toIndex)
{
    if (fromIndex < 1 || fromIndex >= effects.size() || toIndex < 1 || toIndex >= effects.size()) {
        qDebug() << "Ungültige Indizes!";
        return;
    }

    EffectAudioSource* effectToMove = effects[fromIndex];

    effects.erase(effects.begin() + fromIndex);
    effects.insert(effects.begin() + toIndex, effectToMove);

    for (size_t i = 1; i < effects.size(); ++i) {
        effects[i - 1]->setSource(effects[i]);
    }

    qDebug() << "Effekt verschoben von Index" << fromIndex << "nach Index" << toIndex;
}

void AudioEffectService::clearEffects() {
    for (auto* effect : effects) {
        delete effect;
    }
    effects.clear();
}

juce::AudioSource* AudioEffectService::getOutputSource() {
    return effects.empty() ? nullptr : effects.front();
}

QStringList AudioEffectService::getUsingEffects() {
    QStringList list;
    for (auto* effect : effects) {
        switch(effect->getAudioEffectType()) {
        case AudioEffectType::Delay:
            list.append("Delay");
            break;
        case AudioEffectType::Distortion:
            list.append("Distortion");
            break;
        case AudioEffectType::Raw:
            list.append("Raw");
            break;
        case AudioEffectType::Reverb:
            list.append("Reverb");
            break;
        default:
            qDebug("No Effect Found");
        }
    }
    return list;
}
