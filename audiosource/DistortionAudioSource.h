#ifndef DISTORTIONAUDIOSOURCE_H
#define DISTORTIONAUDIOSOURCE_H

#include "EffectAudioSource.h"

class DistortionAudioSource : public EffectAudioSource
{
public:
    DistortionAudioSource() {
        setAudioEffectType(AudioEffectType::Distortion);
    }
    DistortionAudioSource(juce::AudioSource* source) : EffectAudioSource(source) {
        setAudioEffectType(AudioEffectType::Distortion);
    }
    ~DistortionAudioSource() {}

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        getCurrentSource()->getNextAudioBlock(bufferToFill);


        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            {
                auto* channelData = bufferToFill.buffer->getWritePointer(channel);
                auto* processedBuffer = bufferToFill.buffer->getReadPointer(channel);

                float input = processedBuffer[sample];
                float output = input * 5.8f;

                if (output < -1.f) {
                    output =  -1.f;
                }
                else if (output > 1.f) {
                    output = 1.f;
                }

                channelData[sample] =  (1.f - 0.8f) * input + 0.8f * output;
            }
        }
    }

    void releaseResources() override
    {
    }
};

#endif // DISTORTIONAUDIOSOURCE_H
