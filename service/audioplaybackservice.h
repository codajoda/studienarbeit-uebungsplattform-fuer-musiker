#ifndef AUDIOPLAYBACKSERVICE_H
#define AUDIOPLAYBACKSERVICE_H

#include <QDebug>
#include <juce_audio_devices/juce_audio_devices.h>
#include <vector>
#include <juce_audio_formats/juce_audio_formats.h>
#include "Recording.h"
#include "track.h"

class AudioController;

class AudioPlaybackService : public QObject, public juce::MixerAudioSource
{
    Q_OBJECT

public:
    explicit AudioPlaybackService(juce::AudioDeviceManager& sharedDeviceManager, AudioController& audioController);
    ~AudioPlaybackService();

    bool addToTrackList(const Recording& recording);
    void removeFromTrackList(int trackIndex);
    void clearTrackList();

    void start();
    void pause();
    void stop();
    bool isPlaying();

    double getReferencePlaybackPosition();
    double getTrackPlaybackPosition(int trackId);
    void setPlaybackPosition(double timeStamp);

    void setMainVolume(double gain);
    void setTrackVolume(int trackId, int volume);

    void setTrackMuted(int trackId, bool muted);
    void setTrackSolo(int trackId, bool solo);

    void muteAll();
    void unmuteAll();

    std::vector<Track*> getTracks();

public slots:
    void onReferencePlayingChanged(bool isPlaying);

signals:
    void referencePlayingChanged(bool isPlaying);

private:
    AudioController& audioController;
    juce::AudioDeviceManager& deviceManager;
    juce::AudioFormatManager formatManager;
    juce::AudioSourcePlayer audioSourcePlayer;
    std::vector<Track*> tracks;
    Track* reference = nullptr;
    int numSoloTracks;
    double gain;
};

#endif // AUDIOPLAYBACKSERVICE_H
