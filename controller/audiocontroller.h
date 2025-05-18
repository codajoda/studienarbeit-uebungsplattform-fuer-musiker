#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include "audioeffectservice.h"
#include "audiomonitoringservice.h"
#include "audioplaybackservice.h"
#include "audiomanagerservice.h"
#include "audiorecordservice.h"
#include "dbcontroller.h"
#include "waveformservice.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include <QObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <QPointF>
#include <QtCharts/qabstractseries.h>
#include <QtCharts/qxyseries.h>

Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QList<QPointF>)

class AudioController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit AudioController(QObject *parent = nullptr);
    ~AudioController();

    Q_ENUM(AudioEffectType)

    //Audio-Manager-Service:
    Q_INVOKABLE QStringList getAvailableInputDevices();
    Q_INVOKABLE QStringList getAvailableInputChannels();
    Q_INVOKABLE QStringList getAvailableOutputDevices();
    Q_INVOKABLE QStringList getAvailableOutputChannels();
    Q_INVOKABLE QStringList getAvailableSampleRates();
    Q_INVOKABLE QStringList getAvailableBufferSizes();

    Q_INVOKABLE QString getCurrentInputDevice();
    Q_INVOKABLE QStringList getCurrentInputChannels();
    Q_INVOKABLE QString getCurrentOutputDevice();
    Q_INVOKABLE QStringList getCurrentOutputChannels();
    Q_INVOKABLE QString getCurrentSampleRate();
    Q_INVOKABLE QString getCurrentBufferSize();

    Q_INVOKABLE void setInputDevice(const QString& deviceName);
    Q_INVOKABLE bool isInputChannelActive(QString channel);
    Q_INVOKABLE void setInputChannel(int channel, bool active);
    Q_INVOKABLE void setOutputDevice(const QString& deviceName);
    Q_INVOKABLE bool isOutputChannelActive(QString channel);
    Q_INVOKABLE void setOutputChannel(int channel, bool active);
    Q_INVOKABLE void setSampleRate(QString sampleRate);
    Q_INVOKABLE void setBufferSize(QString bufferSize);

    //Audio-Playback-Service:
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    Q_INVOKABLE double getCurrentPlayheadPosition();
    Q_INVOKABLE void setPlayheadPosition(double timeStamp);

    Q_INVOKABLE void addTracks(QVariantList list);
    Q_INVOKABLE void removeTrack(int trackId);
    Q_INVOKABLE void updateTrack(int trackId, bool isMuted, bool isSolo);
    Q_INVOKABLE void muteAll();
    Q_INVOKABLE void unmuteAll();
    Q_INVOKABLE void clearTracks();

    Q_INVOKABLE void setMainVolume(double gain);
    Q_INVOKABLE void setTrackVolume(int trackId, int volume);

    //Audio-Effect-Service:
    Q_INVOKABLE QStringList getUsedEffects();
    Q_INVOKABLE void addEffect(QString type);
    Q_INVOKABLE void removeEffect(int index);
    Q_INVOKABLE void moveEffectFromTo(int from, int to);

    //Audio-Record-Service:
    Q_INVOKABLE void startRecord(int projectId);
    Q_INVOKABLE void stopRecord(int projectId);

    //Audio-Monitoring-Service:
    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void setInputVolume(float gain);

    //Wave-Form-Service:
    Q_INVOKABLE QVariantList getWaveformForFile(const int trackId);

public slots:
    void onReferencePlayingChanged(bool isPlaying);
    void onPitchSent(float pitch);

signals:
    //Audio-Playback-Service:
    void soloChanged(bool solo, int trackId);
    void allMuteChanged(bool mute);
    void playingFinished();

    //Audio-Record-Service:
    void recordingStarted();
    void recordingFinished(Recording* newRecording);

    //Wave-Form-Service:
    void newWaveform(QVector<float> waveform);
    void sendPitch(float pitch);

private:
    juce::AudioDeviceManager sharedDeviceManager;
    std::unique_ptr<AudioManagerService> audioManagerService;
    std::shared_ptr<AudioPlaybackService> audioPlaybackService;
    std::shared_ptr<AudioEffectService> audioEffectService;
    std::unique_ptr<AudioRecordService> audioRecordService;
    std::unique_ptr<AudioMonitoringService> audioCapturingService;
    std::unique_ptr<WaveformService> waveFormService;
    DBController* getProjectController();
};

#endif // AUDIOCONTROLLER_H
