#include "tagservice.h"
#include "Tag.h"
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>

TagService::TagService() {}
TagService::~TagService() {}

QVariantList TagService::getTags() {
    QVariantList tagList;

    QString queryStr = "SELECT id, name FROM Tags";

    QSqlQuery query;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Fehler beim Abrufen der Tags:" << query.lastError().text();
        return tagList;
    }
    while (query.next()) {
        Tag *tag = new Tag(
            query.value("id").toInt(),
            query.value("name").toString()
            );

        tagList.append(QVariant::fromValue(tag));
    }
    return tagList;
}

QVariantList TagService::getTagsForProject(int projectId) {
    QVariantList tagList;

    QString queryStr = "SELECT Tags.id, Tags.name "
                       "FROM Tags "
                       "JOIN projects_tags ON Tags.id = projects_tags.tag_id "
                       "WHERE projects_tags.project_id = :projectId";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":projectId", projectId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Abrufen der Tags für das Projekt:" << query.lastError().text();
        return tagList;
    }

    while (query.next()) {
        Tag *tag = new Tag(
            query.value("id").toInt(),
            query.value("name").toString()
            );

        tagList.append(QVariant::fromValue(tag));
    }

    return tagList;
}

bool TagService::addTag(QString tagName) {
    QString queryStr = "INSERT INTO Tags (name) "
                       "VALUES (:name)";

    QSqlQuery query;
    query.prepare(queryStr);

    query.bindValue(":name", tagName);

    if (!query.exec()) {
        qDebug() << "Fehler beim Hinzufügen des Tags:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Tag erfolgreich hinzugefügt!";
        return true;
    }
}

bool TagService::attachTagToProject(int projectId, QString tagName) {
    QString checkTagQuery = "SELECT id FROM Tags WHERE name = :name";
    QSqlQuery checkTag;
    checkTag.prepare(checkTagQuery);
    checkTag.bindValue(":name", tagName);

    int tagId = -1;
    if (checkTag.exec() && checkTag.next()) {
        tagId = checkTag.value(0).toInt();
    } else {
        QString insertTagQuery = "INSERT INTO Tags (name) VALUES (:name)";
        QSqlQuery insertTag;
        insertTag.prepare(insertTagQuery);
        insertTag.bindValue(":name", tagName);

        if (!insertTag.exec()) {
            qDebug() << "Fehler beim Hinzufügen des Tags:" << insertTag.lastError().text();
            return false;
        }

        tagId = insertTag.lastInsertId().toInt();
    }

    QString insertProjectTagQuery = "INSERT INTO projects_tags (project_id, tag_id) VALUES (:projectId, :tagId)";
    QSqlQuery insertProjectTag;
    insertProjectTag.prepare(insertProjectTagQuery);
    insertProjectTag.bindValue(":projectId", projectId);
    insertProjectTag.bindValue(":tagId", tagId);

    if (!insertProjectTag.exec()) {
        qDebug() << "Fehler beim Hinzufügen des Tags zum Projekt:" << insertProjectTag.lastError().text();
        return false;
    }

    qDebug() << "Tag erfolgreich zum Projekt hinzugefügt!";
    return true;
}

bool TagService::removeTagFromProject(int projectId, QString tagName) {
    QString checkTagQuery = "SELECT id FROM Tags WHERE name = :name";
    QSqlQuery checkTag;
    checkTag.prepare(checkTagQuery);
    checkTag.bindValue(":name", tagName);

    int tagId = -1;
    if (checkTag.exec() && checkTag.next()) {
        tagId = checkTag.value(0).toInt();
    } else {
        qDebug() << "Fehler: Tag existiert nicht";
        return false;
    }

    QString deleteProjectTagQuery = "DELETE FROM projects_tags WHERE project_id = :projectId AND tag_id = :tagId";
    QSqlQuery deleteProjectTag;
    deleteProjectTag.prepare(deleteProjectTagQuery);
    deleteProjectTag.bindValue(":projectId", projectId);
    deleteProjectTag.bindValue(":tagId", tagId);

    if (!deleteProjectTag.exec()) {
        qDebug() << "Fehler beim Entfernen des Tags aus dem Projekt:" << deleteProjectTag.lastError().text();
        return false;
    }

    qDebug() << "Tag erfolgreich aus dem Projekt entfernt!";
    return true;
}

