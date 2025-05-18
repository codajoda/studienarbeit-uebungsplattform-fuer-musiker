#include "audiorecordservice.h"
#include <QtCore/qdebug.h>

AudioRecordService::AudioRecordService(juce::AudioSource& inputSource, juce::AudioDeviceManager& sharedAudioDeviceManager, AudioController &audioController)
    : inputSource(inputSource), audioDeviceManager(sharedAudioDeviceManager), audioController(audioController), backgroundThread("Audio Recorder Thread")
{
    backgroundThread.startThread();
    m_isMonitoring = false;
    m_isRecording = false;
    gain = 1.0f;
}

AudioRecordService::~AudioRecordService()
{
    stopRecording();
}

void AudioRecordService::startRecording(QString path, int sampleRate, int numChannels)
{
    stopRecording();

    m_isRecording = true;

    backgroundThread.startThread();

    outputFile = juce::File(juce::String(path.toStdString()));
    outputFile.deleteFile();
    fileStream = outputFile.createOutputStream();
    if (fileStream == nullptr)
    {
        DBG("Fehler: Konnte Datei-Stream nicht erstellen.");
        return;
    }
    juce::WavAudioFormat wavFormat;

    writer.reset(wavFormat.createWriterFor(fileStream.get(), sampleRate,
                                           static_cast<unsigned int>(numChannels), 16, {}, 0));
    if (writer != nullptr)
    {
        fileStream.release();
        threadedWriter.reset(new juce::AudioFormatWriter::ThreadedWriter(writer.get(),
                                                                         backgroundThread, 100000000));
        writer.release();
        audioDeviceManager.addAudioCallback(this);
    }
    else
    {
        DBG("Fehler: Konnte AudioFormatWriter nicht erstellen.");
    }
}

QString AudioRecordService::stopRecording()
{
    m_isRecording = false;
    audioDeviceManager.removeAudioCallback(this);
    threadedWriter.reset();
    backgroundThread.stopThread(300);
    return QString::fromStdString(outputFile.getFullPathName().toStdString());
}

void AudioRecordService::audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                                          int numInputChannels,
                                                          float* const* outputChannelData,
                                                          int numOutputChannels,
                                                          int numSamples,
                                                          const juce::AudioIODeviceCallbackContext& context)
{
    float* combinedChannel = new float[numSamples];
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float mixedSample = 0.0f;
        for (int inputChan = 0; inputChan < numInputChannels; ++inputChan)
        {
            if (inputChannelData[inputChan] != nullptr)
            {
                mixedSample += inputChannelData[inputChan][sample];
            }
        }
        combinedChannel[sample] = mixedSample;
    }
    juce::AudioBuffer<float> buffer(&combinedChannel, 1, numSamples);
    juce::AudioSourceChannelInfo info(&buffer, 0, numSamples);
    this->inputSource.getNextAudioBlock(info);
    if (threadedWriter != nullptr) {
        float* channels[numOutputChannels];

        float buffers[numSamples];
        for (int sample = 0; sample < numSamples; ++sample) {
            buffers[sample] = buffer.getReadPointer(0)[sample] * gain;
        }
        for (int output = 0; output < numOutputChannels; ++output) {
            channels[output] = buffers;
        }
        threadedWriter->write(channels, numSamples);
    }

    if (m_isMonitoring) {
        for (int outputChan = 0; outputChan < numOutputChannels; ++outputChan)
        {
            auto* outBuffer = outputChannelData[outputChan];
            for (int sample = 0; sample < numSamples; ++sample)
            {
                outBuffer[sample] = combinedChannel[sample] * gain;
            }
        }
    }
    delete[] combinedChannel;
}

void AudioRecordService::audioDeviceAboutToStart (juce::AudioIODevice* device)
{
}

void AudioRecordService::audioDeviceStopped()
{
}

void AudioRecordService::startMonitoring()
{
    m_isMonitoring = true;
}

void AudioRecordService::stopMonitoring()
{
    m_isMonitoring = false;
}

bool AudioRecordService::isMonitoring()
{
    return m_isMonitoring;
}

bool AudioRecordService::isRecording()
{
    return m_isRecording;
}

void AudioRecordService::setInputVolume(float gain)
{
    this->gain = gain;
}
