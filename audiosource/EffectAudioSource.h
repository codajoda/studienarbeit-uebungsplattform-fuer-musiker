#ifndef EFFECTAUDIOSOURCE_H
#define EFFECTAUDIOSOURCE_H

#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>
#include "juce_audio_basics/juce_audio_basics.h"

enum class AudioEffectType {
    Reverb,
    Delay,
    Distortion,
    Chorus,
    Flanger,
    Raw
};



class EffectAudioSource : public juce::AudioSource
{
public:
    EffectAudioSource() {}
    EffectAudioSource(juce::AudioSource* source) {
        setSource(source);
    }
    ~EffectAudioSource() {}

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (getCurrentSource() != nullptr) {
            getCurrentSource()->getNextAudioBlock(bufferToFill);
        }
    }

    void releaseResources() override
    {
    }

    juce::AudioSource* getCurrentSource() {
        return source;
    }

    void setSource(juce::AudioSource* source) {
        this->source = source;
    }

    void setAudioEffectType(AudioEffectType type) {
        this->type = type;
    }

    AudioEffectType getAudioEffectType() {
        return type;
    }


private:
    juce::AudioSource* source;
    AudioEffectType type;
};

#endif // EFFECTAUDIOSOURCE_H
