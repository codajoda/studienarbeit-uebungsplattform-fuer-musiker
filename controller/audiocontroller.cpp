#include "audiocontroller.h"

AudioController::AudioController(QObject *parent)
    : QObject{parent}
{
    sharedDeviceManager.initialiseWithDefaultDevices(2, 2);
    audioManagerService = std::make_unique<AudioManagerService>(sharedDeviceManager);
    audioPlaybackService = std::make_shared<AudioPlaybackService>(sharedDeviceManager, *this);
    audioEffectService = std::make_shared<AudioEffectService>(sharedDeviceManager);
    audioRecordService = std::make_unique<AudioRecordService>(*audioEffectService->getOutputSource(), sharedDeviceManager, *this);
    audioCapturingService = std::make_unique<AudioMonitoringService>(*audioEffectService->getOutputSource(), sharedDeviceManager, *this);
    waveFormService = std::make_unique<WaveformService>();
    connect(audioPlaybackService.get(), &AudioPlaybackService::referencePlayingChanged, this, &AudioController::onReferencePlayingChanged);
}
AudioController::~AudioController() {}

//Audio-Manager-Service:
QStringList AudioController::getAvailableInputDevices()  {return audioManagerService->getAvailableInputDevices();}

QStringList AudioController::getAvailableInputChannels() {return audioManagerService->getAvailableInputChannels();}

QStringList AudioController::getAvailableOutputDevices() {return audioManagerService->getAvailableOutputDevices();}

QStringList AudioController::getAvailableOutputChannels(){return audioManagerService->getAvailableOutputChannels();}

QStringList AudioController::getAvailableSampleRates()   {return audioManagerService->getAvailableSampleRates();}

QStringList AudioController::getAvailableBufferSizes()   {return audioManagerService->getAvailableBufferSizes();}

QString AudioController::getCurrentInputDevice()         {return audioManagerService->getCurrentInputDevice();}

QStringList AudioController::getCurrentInputChannels()   {return audioManagerService->getCurrentInputChannels();}

QString AudioController::getCurrentOutputDevice()        {return audioManagerService->getCurrentOutputDevice();}

QStringList AudioController::getCurrentOutputChannels()  {return audioManagerService->getCurrentOutputChannels();}

QString AudioController::getCurrentSampleRate()          {return audioManagerService->getCurrentSampleRate();}

QString AudioController::getCurrentBufferSize(){return audioManagerService->getCurrentBufferSize();}

void AudioController::setInputDevice(const QString& deviceName)   {audioManagerService->setInputDevice(deviceName);}

bool AudioController::isInputChannelActive(QString channel)  {return audioManagerService->getCurrentInputChannels().contains(channel);}

void AudioController::setInputChannel(int channel, bool active) {audioManagerService->setInputChannel(channel, active);}

void AudioController::setOutputDevice(const QString& deviceName)  {audioManagerService->setOutputDevice(deviceName);}

bool AudioController::isOutputChannelActive(QString channel) {return audioManagerService->getCurrentOutputChannels().contains(channel);}

void AudioController::setOutputChannel(int channel, bool active){audioManagerService->setOutputChannel(channel, active);}

void AudioController::setSampleRate(QString sampleRate)  {audioManagerService->setSampleRate(sampleRate);}

void AudioController::setBufferSize(QString bufferSize)  {audioManagerService->setBufferSize(bufferSize);}

//Audio-Playback-Service:
void AudioController::start() {
    audioPlaybackService->start();
}

void AudioController::pause() {
    audioPlaybackService->pause();
}

void AudioController::stop() {
    audioPlaybackService->pause();
}

double AudioController::getCurrentPlayheadPosition()
{
    return audioPlaybackService->getReferencePlaybackPosition();
}

void AudioController::setPlayheadPosition(double timeStamp)
{
    audioPlaybackService->setPlaybackPosition(timeStamp);
}

void AudioController::addTracks(QVariantList list)
{
    for (const QVariant& item : list) {
        Recording *rec = item.value<Recording*>();
        audioPlaybackService->addToTrackList(*rec);
    }
}

void AudioController::removeTrack(int trackId)
{
    audioPlaybackService->removeFromTrackList(trackId);
}

void AudioController::updateTrack(int trackId, bool isMuted, bool isSolo)
{
    audioPlaybackService->setTrackMuted(trackId, isMuted);
    audioPlaybackService->setTrackSolo(trackId, isSolo);
}

void AudioController::muteAll()
{
    audioPlaybackService->muteAll();
    emit allMuteChanged(true);
}

void AudioController::unmuteAll()
{
    audioPlaybackService->unmuteAll();
    emit allMuteChanged(false);
}

void AudioController::clearTracks() {
    audioPlaybackService->clearTrackList();
}

void AudioController::setMainVolume(double gain) {
    audioPlaybackService->setMainVolume(gain);
}

void AudioController::setTrackVolume(int trackId, int volume)
{
    audioPlaybackService->setTrackVolume(trackId, volume);
}

//Audio-Effect-Service:
QStringList AudioController::getUsedEffects()
{
    return audioEffectService->getUsingEffects();
}

void AudioController::addEffect(QString type)
{
    if (type == "Delay") {
        audioEffectService->addEffect(AudioEffectType::Delay);
    } else if (type == "Reverb") {
        audioEffectService->addEffect(AudioEffectType::Reverb);
    } else if (type == "Distortion") {
        audioEffectService->addEffect(AudioEffectType::Distortion);
    }
}

void AudioController::removeEffect(int index)
{
    audioEffectService->removeEffect(index);
}

void AudioController::moveEffectFromTo(int from, int to)
{
    audioEffectService->moveEffectFromTo(from, to);
}

//Audio-Record-Service:

void AudioController::startRecord(int projectId)
{
    if (audioCapturingService->isRunning()) {
        audioCapturingService->stop();
        audioRecordService->startMonitoring();
    }
    emit recordingStarted();
    QString path = qvariant_cast<QString>(getProjectController()->getProjectFolderAbsolutePath(projectId));
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString fileName =  "recording_" + timestamp + ".wav";
    QString filePath = path + "/" + fileName;
    audioRecordService->startRecording(
        filePath,
        sharedDeviceManager.getCurrentAudioDevice()->getCurrentSampleRate(),
        sharedDeviceManager.getCurrentAudioDevice()->getOutputChannelNames().size()
    );
    getProjectController()->registerAudioFile(filePath, projectId, getCurrentPlayheadPosition());
}

void AudioController::stopRecord(int projectId)
{
    if (audioRecordService->isMonitoring()) {
        audioRecordService->stopMonitoring();
        audioCapturingService->start();
    }
    QString filePath = audioRecordService->stopRecording();

    emit recordingFinished(getProjectController()->getRecordingByFilePath(filePath));
}

void AudioController::onReferencePlayingChanged(bool isPlaying)
{
    if (!isPlaying) {
        emit playingFinished();
    }
}

//Audio-Monitoring-Service:
void AudioController::startMonitoring() {
    if (audioRecordService->isRecording()) {
        audioRecordService->startMonitoring();
    } else {
        audioCapturingService->start();
    }
}

void AudioController::stopMonitoring()  {
    if (audioRecordService->isRecording()) {
        audioRecordService->stopMonitoring();
    } else {
        audioCapturingService->stop();
    }
}

void AudioController::setInputVolume(float gain)
{
    audioCapturingService->setInputVolume(gain);
    audioRecordService->setInputVolume(gain);
}

//Wave-Form-Service:
QVariantList AudioController::getWaveformForFile(const int trackId)
{
    Recording* rec = getProjectController()->getRecordingByTrackId(trackId);

    QVariantList waveformData;
    juce::File audioFile(rec->filePath().toStdString());

    auto rawWaveformData = waveFormService->extractSamplesWithTimestamps(audioFile, 1000, rec->startTime(), getProjectController()->getBackingTrackRecordingFromProject(rec->projectId())->duration());

    for (const auto& [timestamp, sample] : rawWaveformData) {
        QVariantMap point;
        point["timestamp"] = timestamp;
        point["value"] = sample;
        waveformData.append(point);
    }

    return waveformData;
}

//Controller:
DBController* AudioController::getProjectController()
{
    return qmlEngine(this)->singletonInstance<DBController*>("com.example", "DBController");
}

//Other:
void AudioController::onPitchSent(float pitch)
{
    if (pitch > 0) {
        emit sendPitch(pitch);
    }
}
