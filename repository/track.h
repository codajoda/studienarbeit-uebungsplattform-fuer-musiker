#ifndef TRACK_H
#define TRACK_H

#include <QtCore/qdebug.h>
#include <QtCore/qobject.h>
#include <juce_audio_devices/juce_audio_devices.h>

class Track : public QObject, public juce::ChangeListener {
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
    Q_PROPERTY(QString filePath READ filePath CONSTANT)
    Q_PROPERTY(int duration READ duration CONSTANT)
    Q_PROPERTY(double offset READ offset CONSTANT)
    Q_PROPERTY(bool isMuted READ isMuted WRITE setMuted)
    Q_PROPERTY(bool isSolo READ isSolo WRITE setSolo)
    Q_PROPERTY(bool isSoloMuted READ isSoloMuted WRITE setSoloMuted)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playingChanged)

public:
    // Konstruktor
    explicit Track(int id, const std::string &fileName, const std::string &filePath, int duration, double offset,
                   juce::AudioTransportSource& transportSource, double gain, QObject* parent = nullptr)
        : QObject(parent), m_id(id), m_fileName(QString::fromStdString(fileName)), m_filePath(QString::fromStdString(filePath)),
        m_duration(duration), m_offset(offset), m_isMuted(false), m_isSolo(false), m_isSoloMuted(false), m_isPlaying(false), transportSource(transportSource), gain(gain)
    {
        transportSource.addChangeListener(this);
    }

    ~Track() {
        qDebug() << "Track destroyed: " << m_fileName;
    }

    int id() const { return m_id; }
    QString fileName() const { return m_fileName; }
    QString filePath() const { return m_filePath; }
    int duration() const { return m_duration; }
    double offset() const { return m_offset; }
    juce::AudioTransportSource& getTransportSource() { return transportSource; }

    bool isSoloMuted() const { return m_isSoloMuted; }
    bool isMuted() const { return m_isMuted; }
    bool isSolo() const { return m_isSolo; }
    bool isPlaying() const { return m_isPlaying; }
    bool hasFinished() const { return transportSource.hasStreamFinished(); }

    void setMuted(bool muted) {
        m_isMuted = muted;
    }

    void setSolo(bool solo) {
        m_isSolo = solo;
        if (solo) {
            m_isSoloMuted = false;
        }
    }

    void setSoloMuted(bool soloMuted) {
        m_isSoloMuted = soloMuted;

    }

    void setVolume(double volume) {
        this->volume = volume;
    }

    void setGain(double gain) {
        this->gain = gain;
    }

    void updateVolume() {
        if (m_isMuted && m_isSolo && !m_isSoloMuted) {
            transportSource.setGain(gain/75 * volume/75);
        } else if (m_isMuted && !m_isSolo && m_isSoloMuted) {
            transportSource.setGain(0);
        } else if (m_isMuted && !m_isSolo && !m_isSoloMuted) {
            transportSource.setGain(0);
        } else if (!m_isMuted && m_isSolo && !m_isSoloMuted) {
            transportSource.setGain(gain/75 * volume/75);
        } else if (!m_isMuted && !m_isSolo && m_isSoloMuted) {
            transportSource.setGain(0);
        } else if (!m_isMuted && !m_isSolo && !m_isSoloMuted) {
            transportSource.setGain(gain/75 * volume/75);
        }
    }

    void start() {
        transportSource.start();
    }
    void stop() {
        transportSource.stop();
    }

    double getCurrentPosition() {
        return transportSource.getCurrentPosition();
    }
    void setPosition(double timeStamp) {
        transportSource.setPosition(timeStamp - m_offset);
    }

signals:
    void playingChanged(bool playing);

private:
    int m_id;
    QString m_fileName;
    QString m_filePath;
    int m_duration;
    double m_offset;
    bool m_isMuted;
    bool m_isSolo;
    bool m_isSoloMuted;
    bool m_isPlaying;
    double volume = 50;
    double gain;

    juce::AudioTransportSource& transportSource;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override {
        if (source == &transportSource) {
            m_isPlaying = transportSource.isPlaying();
            emit playingChanged(transportSource.isPlaying());
        }
    }
};

#endif // TRACK_H
