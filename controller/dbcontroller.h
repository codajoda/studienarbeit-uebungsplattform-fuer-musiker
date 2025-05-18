#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include "projectservice.h"
#include "Recording.h"
#include "recordservice.h"
#include "tagservice.h"
#include <QtCore/qobject.h>
#include <QtCore/qtmetamacros.h>
#include <qqmlintegration.h>

class DBController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit DBController(QObject *parent = nullptr);
    ~DBController();

    //Project-Service:
    Q_INVOKABLE bool createProject(const QString project_name, const int beats_per_minute, const QString musical_key, const QString instrumental_file_path, const QString recordings_directory);
    Q_INVOKABLE void deleteProject(int projectId);
    Q_INVOKABLE QVariantList getAllProjects();
    QString getProjectFolderAbsolutePath(int projectId);

    //Record-Service:
    Q_INVOKABLE bool updateTrackName(int trackId, QString name);
    Q_INVOKABLE void deleteTrack(int trackId);
    Q_INVOKABLE QVariantList getRecordings(int projectId);
    void registerAudioFile(QString filePath, int projectId, double startTime);
    Recording *getRecordingByTrackId(int trackId);
    Recording *getRecordingByFilePath(QString filePath);
    Recording *getBackingTrackRecordingFromProject(int projectId);

    //Tag-Service;
    Q_INVOKABLE void attachTagToProject(int projectId, QString tagName);
    Q_INVOKABLE void removeTagFromProject(int projectId, QString tagName);
    Q_INVOKABLE QVariantList getTags();
    Q_INVOKABLE QVariantList getTagsForProject(int projectId);

private:
    std::unique_ptr<ProjectService> projectService;
    std::unique_ptr<RecordService> recordService;
    std::unique_ptr<TagService> tagService;
};

#endif // DBCONTROLLER_H
