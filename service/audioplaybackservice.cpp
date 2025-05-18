#include "audioplaybackservice.h"
#include "audiocontroller.h"
#include "juce_audio_devices/juce_audio_devices.h"

AudioPlaybackService::AudioPlaybackService(juce::AudioDeviceManager& sharedDeviceManager, AudioController &audioController)
    : deviceManager(sharedDeviceManager), audioController(audioController)
{
    formatManager.registerBasicFormats();
    deviceManager.addAudioCallback(&audioSourcePlayer);
    audioSourcePlayer.setSource(this);
    numSoloTracks = 0;
    gain = 50.0;
}

AudioPlaybackService::~AudioPlaybackService()
{
    deviceManager.removeAudioCallback(&audioSourcePlayer);
}

bool AudioPlaybackService::addToTrackList(const Recording& recording) {

    qDebug() << recording.displayName();
    juce::File audioFile(recording.filePath().toStdString());

    // Lade die Datei
    auto* reader = formatManager.createReaderFor(audioFile);
    if (reader == nullptr) {
        qDebug() << "Fehler: Datei konnte nicht geladen werden!";
        return false;
    }

    juce::AudioFormatReaderSource* readerSource = new juce::AudioFormatReaderSource(reader, true);
    juce::AudioTransportSource* newTransportSource = new juce::AudioTransportSource();

    newTransportSource->setSource(readerSource, 0, nullptr, reader->sampleRate);

    qDebug() << gain;
    Track* newTrack = new Track(recording.id(), recording.fileName().toStdString(), recording.filePath().toStdString(),
                                recording.duration(), recording.startTime() / 1000.0, *newTransportSource, gain);
    if (numSoloTracks > 0) {
        newTrack->setSoloMuted(true);
    }
    newTrack->updateVolume();

    tracks.push_back(newTrack);

    if (recording.isBackingTrack()) {
        reference = newTrack;
        connect(reference, &Track::playingChanged, this, &AudioPlaybackService::onReferencePlayingChanged);
    }

    this->addInputSource(newTransportSource, false);

    return true;
}

void AudioPlaybackService::removeFromTrackList(int trackIndex)
{
    for (int i = 0; i < tracks.size(); i++) {
        if (tracks.at(i)->id() == trackIndex) {
            setTrackSolo(trackIndex, false);
            this->removeInputSource(&tracks.at(i)->getTransportSource());
            //tracks.at(i)->stop();
            tracks.erase(tracks.begin() + i);
        }
    }

}

void AudioPlaybackService::clearTrackList() {
    stop();
    tracks.clear();
}


void AudioPlaybackService::start()
{
    if (reference->hasFinished()) {
        setPlaybackPosition(0);
    } else {
        setPlaybackPosition(getReferencePlaybackPosition());

    }
    for (auto& track : tracks)
    {
        track->start();
        qDebug() << "Wiedergabe gestartet für Track: " << track->fileName();
    }
}

void AudioPlaybackService::pause()
{
    for (auto& track : tracks)
    {

        track->stop();
        qDebug() << "Wiedergabe pausiert für Track: " << track->fileName();
    }
}

void AudioPlaybackService::stop()
{
    for (auto& track : tracks)
    {
        track->stop();
        track->setPosition(0);
        qDebug() << "Wiedergabe gestoppt für Track: " << track->fileName();
    }
}

bool AudioPlaybackService::isPlaying()
{
    return reference->isPlaying();
}

double AudioPlaybackService::getReferencePlaybackPosition()
{
    return reference->getCurrentPosition();
}


double AudioPlaybackService::getTrackPlaybackPosition(int trackId)
{
    for (auto& track : tracks) {
        if (track->id() == trackId) {
            return track->getCurrentPosition();
        }
    }
    return 0;
}

void AudioPlaybackService::setPlaybackPosition(double timeStamp)
{
    for (auto& track : tracks) {
        track->setPosition(timeStamp);
        if (reference->isPlaying()) {
            track->start();
        }
    }
}

void AudioPlaybackService::setMainVolume(double gain)
{
    this->gain = gain;
    for (auto& track : tracks) {
        track->setGain(gain);
    }
    for (auto& track : tracks)
    {
        track->updateVolume();
    }
}

void AudioPlaybackService::setTrackVolume(int trackId, int volume)
{
    for (auto& track : tracks) {
        if (track->id() == trackId) {
            track->setVolume(volume);
        }
    }
    for (auto& track : tracks)
    {
        track->updateVolume();
    }
}

void AudioPlaybackService::setTrackMuted(int trackIndex, bool muted)
{
    for (auto& track : tracks)
    {
        if (track->id() == trackIndex) {
            track->setMuted(muted);
            break;
        }
    }
    for (auto& track : tracks)
    {
        track->updateVolume();
    }
}

void AudioPlaybackService::setTrackSolo(int trackIndex, bool solo)
{
    for (auto& track : tracks) {
        if (track->id() == trackIndex && track->isSolo() != solo) {
            numSoloTracks += solo ? 1 : -1;
            track->setSolo(solo);
            emit audioController.soloChanged(solo, trackIndex);
            break;
        }
    }
    if (numSoloTracks == 0) {
        for (auto& track : tracks) {
            track->setSoloMuted(false);
        }
    } else {
        for (auto& track : tracks) {
            if (!track->isSolo()) {
                track->setSoloMuted(true);
            }
        }
    }
    for (auto& track : tracks)
    {
        track->updateVolume();
    }
}

void AudioPlaybackService::muteAll()
{
    for (auto& track : tracks)
    {
        track->setMuted(true);
    }
    for (auto& track : tracks)
    {
        track->updateVolume();
    }
}

void AudioPlaybackService::unmuteAll()
{
    for (auto& track : tracks)
    {
        track->setMuted(false);
    }
    for (auto& track : tracks)
    {
        track->updateVolume();
    }
}

std::vector<Track*> AudioPlaybackService::getTracks() {
    return tracks;
}

void AudioPlaybackService::onReferencePlayingChanged(bool isPlaying)
{
    emit referencePlayingChanged(isPlaying);
}
