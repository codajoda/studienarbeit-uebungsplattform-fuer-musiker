#include "projectservice.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSharedPointer>
#include <QtCore/qdatetime.h>
#include <QFileInfo>

ProjectService::ProjectService() {}

ProjectService::~ProjectService() {}

bool ProjectService::refreshAllProjects()
{
    QString queryStr = "SELECT id, file_path FROM Recordings";
    QSqlQuery query;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Fehler beim Abrufen der Recordings:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        int recordingId = query.value("id").toInt();
        QString filePath = query.value("file_path").toString();

        QFileInfo checkFile(filePath);
        if (!checkFile.exists()) {
            QString deleteQuery = "DELETE FROM Recordings WHERE id = :recordingId";
            QSqlQuery deleteRecordingQuery;
            deleteRecordingQuery.prepare(deleteQuery);
            deleteRecordingQuery.bindValue(":recordingId", recordingId);

            if (!deleteRecordingQuery.exec()) {
                qDebug() << "Fehler beim Löschen des Recordings mit ID " << recordingId << ":" << deleteRecordingQuery.lastError().text();
            } else {
                qDebug() << "Recording mit ID " << recordingId << " wurde gelöscht, da die Datei nicht existiert.";
            }
        }
    }

    return true;
}

QVariantList ProjectService::getAllProjects() {
    QString queryStr = "SELECT * FROM Projects";
    QSqlQuery query;
    query.prepare(queryStr);

    QVariantList projects;
    if (query.exec()) {
        while (query.next()) {
            Project *project = new Project(
                query.value("project_id").toInt(),
                query.value("recordings_directory").toString(),
                query.value("project_name").toString(),
                query.value("beats_per_minute").toInt(),
                query.value("musical_key").toString(),
                query.value("created_at").toString()
                );
            projects.append(QVariant::fromValue(project));
        }
    } else {
        qDebug() << "Fehler beim Abrufen der Projekte:" << query.lastError().text();
    }
    return projects;
}

Project *ProjectService::getProjectById(int projectId) {
    QString queryStr = "SELECT * FROM project WHERE project_id = :projectId";
    QSqlQuery query;

    query.prepare(queryStr);
    query.bindValue(":projectId", projectId);

    if (query.exec() && query.next()) {
        Project *project = new Project(
            query.value("project_id").toInt(),
            query.value("recordings_directory").toString(),
            query.value("project_name").toString(),
            query.value("beats_per_minute").toInt(),
            query.value("musical_key").toString(),
            query.value("created_at").toString()
            );
        return project;
    }
    return 0;
}

QString ProjectService::getProjectFolderAbsolutePath(int projectId) {
    QString queryStr = "SELECT recordings_directory FROM Projects WHERE project_id = :projectId";
    QSqlQuery query;

    query.prepare(queryStr);
    query.bindValue(":projectId", projectId);

    QString path;

    if (query.exec()) {
        if (query.next()) {
            path = query.value("recordings_directory").toString();
        }
    } else {
        qDebug() << "Fehler beim Abrufen der Projekte:" << query.lastError().text();
    }
    return path;
}

int ProjectService::createProject(const QString project_name, const int beats_per_minute, const QString musical_key, const QString recordings_directory) {
    QString queryStr = "INSERT INTO Projects (recordings_directory, project_name, beats_per_minute, musical_key, created_at) "
                       "VALUES (:recordings_directory, :project_name, :beats_per_minute, :musical_key, :created_at)";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":recordings_directory", recordings_directory);
    query.bindValue(":project_name", project_name);
    query.bindValue(":beats_per_minute", beats_per_minute);
    query.bindValue(":musical_key", musical_key);
    query.bindValue(":created_at", QDateTime::currentDateTime());

    if (!query.exec()) {
        qDebug() << "Fehler beim Erstellen des Projekts:" << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

bool ProjectService::deleteProject(int projectId) {
    QString queryStr = "DELETE FROM Projects WHERE project_id = :projectId";
    QString queryStr2 = "DELETE FROM Recordings WHERE project_id = :projectId";
    QString queryStr3 = "DELETE FROM projects_tags WHERE project_id = :projectId";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":projectId", projectId);
    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen des Projekts:" << query.lastError().text();
        return false;
    }
    query.prepare(queryStr2);
    query.bindValue(":projectId", projectId);
    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen des Projekts:" << query.lastError().text();
        return false;
    }
    query.prepare(queryStr3);
    query.bindValue(":projectId", projectId);
    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen des Projekts:" << query.lastError().text();
        return false;
    }
    return true;
}
