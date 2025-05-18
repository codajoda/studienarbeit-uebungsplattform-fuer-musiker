#include "recordservice.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "Recording.h"
#include <QtCore/qdebug.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <QFileInfo>

RecordService::RecordService() {}

RecordService::~RecordService() {}

QVariantList RecordService::getRecordings(int projectId) {
    QVariantList recordingsList;

    QString queryStr = "SELECT id, file_name, display_name, file_path, project_id, recording_date, start_time, is_backing_track, duration "
                       "FROM Recordings WHERE project_id = :projectId";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":projectId", projectId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Abrufen der Recordings:" << query.lastError().text();
        return recordingsList;
    }
    while (query.next()) {
        Recording *rec = new Recording(
            query.value("id").toInt(),
            query.value("file_name").toString(),
            query.value("display_name").toString(),
            query.value("file_path").toString(),
            query.value("project_id").toInt(),
            query.value("recording_date").toString(),
            query.value("start_time").toInt(),
            query.value("is_backing_track").toInt(),
            query.value("duration").toInt()
            );

        recordingsList.append(QVariant::fromValue(rec));
    }
    return recordingsList;
}

QVariant RecordService::getRecordingById(int trackId)
{
    QString queryStr = "SELECT id, file_name, display_name, file_path, project_id, recording_date, start_time, is_backing_track, duration "
                       "FROM Recordings WHERE id = :trackId";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":trackId", trackId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Abrufen der Recordings:" << query.lastError().text();
    }
    if (query.next()) {
        Recording *rec = new Recording(
            query.value("id").toInt(),
            query.value("file_name").toString(),
            query.value("display_name").toString(),
            query.value("file_path").toString(),
            query.value("project_id").toInt(),
            query.value("recording_date").toString(),
            query.value("start_time").toInt(),
            query.value("is_backing_track").toInt(),
            query.value("duration").toInt()
            );
        return QVariant::fromValue(rec);
    }
    return QVariant();
}

QVariant RecordService::getRecordingByFilePath(QString filePath)
{
    QString queryStr = "SELECT id, file_name, display_name, file_path, project_id, recording_date, start_time, is_backing_track, duration "
                       "FROM Recordings WHERE file_path = :file_path";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":file_path", filePath);

    if (!query.exec()) {
        qDebug() << "Fehler beim Abrufen der Recordings:" << query.lastError().text();
    }
    if (query.next()) {
        Recording *rec = new Recording(
            query.value("id").toInt(),
            query.value("file_name").toString(),
            query.value("display_name").toString(),
            query.value("file_path").toString(),
            query.value("project_id").toInt(),
            query.value("recording_date").toString(),
            query.value("start_time").toInt(),
            query.value("is_backing_track").toInt(),
            query.value("duration").toInt()
            );
        return QVariant::fromValue(rec);
    }
    return QVariant();
}

QVariant RecordService::getBackingTrackRecording(int projectId)
{
    QString queryStr = "SELECT id, file_name, display_name, file_path, project_id, recording_date, start_time, is_backing_track, duration "
                       "FROM Recordings WHERE project_id = :projectId AND is_backing_track = :isBackingTrack";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":projectId", projectId);
    query.bindValue(":isBackingTrack", 1);

    if (!query.exec()) {
        qDebug() << "Fehler beim Abrufen der Recordings:" << query.lastError().text();
    }
    if (query.next()) {
        Recording *rec = new Recording(
            query.value("id").toInt(),
            query.value("file_name").toString(),
            query.value("display_name").toString(),
            query.value("file_path").toString(),
            query.value("project_id").toInt(),
            query.value("recording_date").toString(),
            query.value("start_time").toInt(),
            query.value("is_backing_track").toInt(),
            query.value("duration").toInt()
            );
        return QVariant::fromValue(rec);
    }
    return QVariant();
}

bool RecordService::updateTrackName(int trackId, QString name)
{
    QString queryStr = "UPDATE Recordings SET display_name = :displayName WHERE id = :trackId";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":displayName", name);
    query.bindValue(":trackId", trackId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Aktualisieren des Track-Namens:" << query.lastError().text();
        return false;
    }
    return true;
}

void RecordService::deleteTrack(int trackId)
{
    QString getFilePathQueryStr = "SELECT file_path FROM Recordings WHERE id = :trackId";

    QSqlQuery query;
    query.prepare(getFilePathQueryStr);
    query.bindValue(":trackId", trackId);

    QString filePath;
    if (query.exec() && query.next()) {
        filePath = query.value("file_path").toString();
    } else {
        qDebug() << "Fehler: Konnte den Dateipfad nicht abrufen:" << query.lastError().text();
        return;
    }

    QFile file(filePath);
    if (file.exists()) {
        if (file.remove()) {
            qDebug() << "Datei erfolgreich gelöscht: " << filePath;
        } else {
            qDebug() << "Fehler beim Löschen der Datei: " << filePath;
        }
    }

    QString deleteQueryStr = "DELETE FROM Recordings WHERE id = :trackId";
    query.prepare(deleteQueryStr);
    query.bindValue(":trackId", trackId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen des Tracks aus der Datenbank:" << query.lastError().text();
    } else {
        qDebug() << "Track erfolgreich aus der Datenbank gelöscht.";
    }
}

void RecordService::registerNewTrack(QString filePath, int projectId, double startTime, bool isBackingTrack)
{
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    QString displayName = fileInfo.baseName();

    QString recordingDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    int duration = 0;
    if (isBackingTrack) {
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(juce::File(filePath.toStdString())));

        if (reader != nullptr)
        {
            double durationInSeconds = reader->lengthInSamples / reader->sampleRate;
            duration = durationInSeconds * 1000.0;
        }
        else
        {
            juce::Logger::writeToLog("Fehler: Datei konnte nicht geladen werden.");
        }
    }

    QString queryStr = "INSERT INTO Recordings (file_name, display_name, file_path, project_id, recording_date, start_time, is_backing_track, duration) "
                       "VALUES (:file_name, :display_name, :file_path, :project_id, :recording_date, :start_time, :is_backing_track, :duration)";

    QSqlQuery query;
    query.prepare(queryStr);

    query.bindValue(":file_name", fileName);
    query.bindValue(":display_name", displayName);
    query.bindValue(":file_path", filePath);
    query.bindValue(":project_id", projectId);
    query.bindValue(":recording_date", recordingDate);
    query.bindValue(":start_time", startTime * 1000); //Sekunden -> Millisekunden
    query.bindValue(":is_backing_track", isBackingTrack ? 1 : 0);
    query.bindValue(":duration", duration);

    if (!query.exec()) {
        qDebug() << "Fehler beim Hinzufügen der Aufnahme:" << query.lastError().text();
    } else {
        qDebug() << "Aufnahme erfolgreich hinzugefügt!";
    }
}
