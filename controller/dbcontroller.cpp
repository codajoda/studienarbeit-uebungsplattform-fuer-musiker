#include "dbcontroller.h"
#include <QQmlEngine>
#include <QtCore/qdebug.h>
#include <QtQml/qqmlapplicationengine.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>

DBController::DBController(QObject *parent)
    : QObject{parent}
{
    projectService = std::make_unique<ProjectService>();
    recordService = std::make_unique<RecordService>();
    tagService = std::make_unique<TagService>();
}

DBController::~DBController() {

}

//Project-Service:
bool DBController::createProject(const QString project_name, const int beats_per_minute, const QString musical_key, const QString instrumental_file_path, const QString recordings_directory) {
    int projectId = projectService->createProject(project_name, beats_per_minute, musical_key, recordings_directory);
    recordService->registerNewTrack(instrumental_file_path, projectId, 0, true);
    return true;
}
void DBController::deleteProject(int projectId)
{
    projectService->deleteProject(projectId);
}
QVariantList DBController::getAllProjects() {
    projectService->refreshAllProjects();
    return projectService->getAllProjects();
}
QString DBController::getProjectFolderAbsolutePath(int projectId) {
    return projectService->getProjectFolderAbsolutePath(projectId);
}

//Record-Service:
bool DBController::updateTrackName(int trackId, QString name)
{
    return recordService->updateTrackName(trackId, name);
}

void DBController::deleteTrack(int trackId)
{
    recordService->deleteTrack(trackId);
}

QVariantList DBController::getRecordings(int projectId) {
    QVariantList recordingsList = recordService->getRecordings(projectId);

    return recordingsList;
}
void DBController::registerAudioFile(QString filePath, int projectId, double startTime)
{
    return recordService->registerNewTrack(filePath, projectId, startTime, false);
}
Recording* DBController::getRecordingByTrackId(int trackId)
{
    return recordService->getRecordingById(trackId).value<Recording*>();
}

Recording* DBController::getRecordingByFilePath(QString filePath)
{
    return recordService->getRecordingByFilePath(filePath).value<Recording*>();

}
Recording* DBController::getBackingTrackRecordingFromProject(int projectId)
{
    return recordService->getBackingTrackRecording(projectId).value<Recording*>();
}

//Tag-Service:
void DBController::attachTagToProject(int projectId, QString tagName) {
    tagService->attachTagToProject(projectId, tagName);
}
void DBController::removeTagFromProject(int projectId, QString tagName)
{
    tagService->removeTagFromProject(projectId, tagName);
}
QVariantList DBController::getTags()
{
    return tagService->getTags();
}
QVariantList DBController::getTagsForProject(int projectId)
{
    return tagService->getTagsForProject(projectId);
}
