#ifndef PROJECT_H
#define PROJECT_H

#include <QtCore/qobject.h>

class Project : public QObject {
    Q_OBJECT
    Q_PROPERTY(int projectId READ projectId CONSTANT)
    Q_PROPERTY(QString recordingsDirectory READ recordingsDirectory CONSTANT)
    Q_PROPERTY(QString projectName READ projectName CONSTANT)
    Q_PROPERTY(int beatsPerMinute READ beatsPerMinute CONSTANT)
    Q_PROPERTY(QString musicalKey READ musicalKey CONSTANT)
    Q_PROPERTY(QString createdAt READ createdAt CONSTANT);
public:
    explicit Project(int projectId, const QString &recordingsDirectory, const QString &projectName, const int &beatsPerMinute,
                     QString musicalKey, const QString &createdAt,
                     QObject *parent = nullptr)
        : QObject(parent), m_projectId(projectId), m_recordingsDirectory(recordingsDirectory), m_projectName(projectName), m_beatsPerMinute(beatsPerMinute),
        m_musicalKey(musicalKey), m_createdAt(createdAt) {}

    int projectId() const { return m_projectId; }
    QString recordingsDirectory() const { return m_recordingsDirectory; }
    QString projectName() const { return m_projectName; }
    int beatsPerMinute() const { return m_beatsPerMinute; }
    QString musicalKey() const { return m_musicalKey; }
    QString createdAt() const { return m_createdAt; }

private:
    int m_projectId;
    QString m_recordingsDirectory;
    QString m_projectName;
    int m_beatsPerMinute;
    QString m_musicalKey;
    QString m_createdAt;
};

#endif // PROJECT_H
