#ifndef AUDIORECORDSERVICE_H
#define AUDIORECORDSERVICE_H

#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include <QString>

class AudioController;

class AudioRecordService : public juce::AudioIODeviceCallback
{
public:
    explicit AudioRecordService(juce::AudioSource& inputSource, juce::AudioDeviceManager& sharedAudioDeviceManager, AudioController& audioController);
    ~AudioRecordService() override;

    void startRecording(QString path, int sampleRate, int numChannels);
    QString stopRecording();

    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                          int numInputChannels,
                                          float* const* outputChannelData,
                                          int numOutputChannels,
                                          int numSamples,
                                          const juce::AudioIODeviceCallbackContext& context) override;
    void audioDeviceAboutToStart (juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;

    void startMonitoring();
    void stopMonitoring();

    bool isMonitoring();
    bool isRecording();

    void setInputVolume(float gain);

private:
    juce::AudioDeviceManager& audioDeviceManager;
    AudioController& audioController;
    juce::AudioSource& inputSource;

    juce::TimeSliceThread backgroundThread;
    std::unique_ptr<juce::AudioFormatWriter::ThreadedWriter> threadedWriter;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    std::unique_ptr<juce::FileOutputStream> fileStream;
    juce::File outputFile;

    bool m_isMonitoring;
    bool m_isRecording;

    float gain;
};

#endif // AUDIORECORDSERVICE_H
