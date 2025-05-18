#ifndef DELAYAUDIOSOURCE_H
#define DELAYAUDIOSOURCE_H

#include "EffectAudioSource.h"
#include "juce_audio_basics/juce_audio_basics.h"

class DelayAudioSource : public EffectAudioSource
{
public:
    DelayAudioSource() {
        setAudioEffectType(AudioEffectType::Delay);
    }
    DelayAudioSource(juce::AudioSource* source) : EffectAudioSource(source) {
        setAudioEffectType(AudioEffectType::Delay);
    }
    ~DelayAudioSource() {
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (getCurrentSource() != nullptr) {
            getCurrentSource()->getNextAudioBlock(bufferToFill);
        }
        const float delayTimeInSeconds = 0.5f;
        const int delaySamples = static_cast<int>(delayTimeInSeconds * 44100.0f);

        static std::vector<float> delayBuffer(bufferToFill.buffer->getNumChannels() * delaySamples, 0.0f);
        static int writeIndex = 0;
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            if (delaySamples == 0) {
                return;
            }

            int readIndex = (writeIndex + delaySamples) % delaySamples;
            for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            {
                auto* channelData = bufferToFill.buffer->getWritePointer(channel);
                auto* processedBuffer = bufferToFill.buffer->getReadPointer(channel);

                channelData[sample] = processedBuffer[sample] + 0.4 * delayBuffer[readIndex + channel * delaySamples];

                delayBuffer[writeIndex + channel * delaySamples] = processedBuffer[sample];
            }
            writeIndex = (writeIndex + 1) % delaySamples;
        }
    }

    void releaseResources() override
    {
    }
};

#endif // DELAYAUDIOSOURCE_H
