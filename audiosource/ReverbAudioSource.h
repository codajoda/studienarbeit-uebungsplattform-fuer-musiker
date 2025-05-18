#ifndef REVERBAUDIOSOURCE_H
#define REVERBAUDIOSOURCE_H

#include <EffectAudioSource.h>
#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>

class ReverbAudioSource : public EffectAudioSource
{
public:
    ReverbAudioSource() {
        setAudioEffectType(AudioEffectType::Reverb);
    }
    ReverbAudioSource(juce::AudioSource* source) : EffectAudioSource(source) {
        setAudioEffectType(AudioEffectType::Reverb);
    }
    ~ReverbAudioSource() {}

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        reverbSource->getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
    }

    void setSource(juce::AudioSource* source) {
        EffectAudioSource::setSource(source);
        reverbSource = new juce::ReverbAudioSource(getCurrentSource(), false);
        juce::Reverb::Parameters reverbParams;
        reverbParams.dryLevel = 0.5f;
        reverbParams.wetLevel = 0.5f;
        reverbParams.roomSize = 0.8f;
        reverbParams.damping = 0.5f;
        reverbParams.width = 1.0f;
        reverbParams.freezeMode = false;
        reverbSource->setParameters(reverbParams);
    }
private:
    juce::ReverbAudioSource* reverbSource;
};

#endif // REVERBAUDIOSOURCE_H
