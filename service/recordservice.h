#ifndef RECORDSERVICE_H
#define RECORDSERVICE_H

#include <QString>
#include <QVector>

class RecordService {
public:
    RecordService();
    ~RecordService();

    QVariantList getRecordings(int projectId);
    QVariant getRecordingById(int trackId);
    QVariant getRecordingByFilePath(QString filePath);
    QVariant getBackingTrackRecording(int projectId);

    bool updateTrackName(int trackId, QString name);
    void deleteTrack(int trackId);
    void registerNewTrack(QString filePath, int projectId, double startTime, bool isBackingTrack);
};

#endif // RECORDSERVICE_H
