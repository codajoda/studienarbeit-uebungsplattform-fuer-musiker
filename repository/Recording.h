#ifndef RECORDING_H
#define RECORDING_H

#include <QtCore/qobject.h>
class Recording : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
    Q_PROPERTY(QString displayName READ displayName CONSTANT)
    Q_PROPERTY(QString filePath READ filePath CONSTANT)
    Q_PROPERTY(int projectId READ projectId CONSTANT)
    Q_PROPERTY(QString recordingDate READ recordingDate CONSTANT)
    Q_PROPERTY(double startTime READ startTime CONSTANT)
    Q_PROPERTY(int isBackingTrack READ isBackingTrack CONSTANT)
    Q_PROPERTY(int duration READ duration CONSTANT);
public:
    explicit Recording(int id, const QString &fileName, const QString &displayName, const QString &filePath,
                       int projectId, const QString &recordingDate, double startTime, int isBackingTrack, int duration,
                       QObject *parent = nullptr)
        : QObject(parent), m_id(id), m_fileName(fileName), m_displayName(displayName), m_filePath(filePath),
        m_projectId(projectId), m_recordingDate(recordingDate), m_startTime(startTime), m_isBackingTrack(isBackingTrack) ,m_duration(duration) {}

    int id() const { return m_id; }
    QString fileName() const { return m_fileName; }
    QString displayName() const { return m_displayName; }
    QString filePath() const { return m_filePath; }
    int projectId() const { return m_projectId; }
    QString recordingDate() const { return m_recordingDate; }
    int startTime() const { return m_startTime; }
    bool isBackingTrack() const { return m_isBackingTrack == 1; }
    int duration() const { return m_duration; }

private:
    int m_id;
    QString m_fileName;
    QString m_displayName;
    QString m_filePath;
    int m_projectId;
    QString m_recordingDate;
    double m_startTime;
    int m_isBackingTrack;
    int m_duration;
};
#endif // RECORDING_H
