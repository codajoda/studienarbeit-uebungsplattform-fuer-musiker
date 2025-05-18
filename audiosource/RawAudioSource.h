#ifndef RAWAUDIOSOURCE_H
#define RAWAUDIOSOURCE_H

#include "EffectAudioSource.h"

class RawAudioSource : public EffectAudioSource
{
public:
    RawAudioSource() {
        setAudioEffectType(AudioEffectType::Raw);
    }
    RawAudioSource(juce::AudioSource* source) : EffectAudioSource(source) {
        setAudioEffectType(AudioEffectType::Raw);
    }
    ~RawAudioSource() {};;
};
#endif // RAWAUDIOSOURCE_H
