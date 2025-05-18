#include "dbrepository.h"
#include "settings.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QtCore/qfileinfo.h>

DBRepository& DBRepository::instance() {
    static DBRepository instance;
    return instance;
}

DBRepository::DBRepository()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString databasePath = Settings::instance().getValue("databasePath").toString();
    db.setDatabaseName(databasePath);

    qDebug() << "Try opening Database...";
    if (!openDatabase()) {
        qDebug() << "Create new Database...";
        createTables();
    }
}

DBRepository::~DBRepository() {
    closeDatabase();
}

bool DBRepository::openDatabase() {
    QFileInfo dbFile(Settings::instance().getValue("databasePath").toString());
    if (!dbFile.exists() || !dbFile.isFile()) {
        db.open();
        return false;
    }
    db.open();
    qDebug() << "Verbindung zur Datenbank hergestellt: " << Settings::instance().getValue("databasePath").toString();
    return true;
}

void DBRepository::closeDatabase() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Datenbank-Verbindung geschlossen.";
    }
}

bool DBRepository::executeQuery(const QString &queryStr) {
    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "SQL-Fehler: " << query.lastError().text();
        return false;
    }
    return true;
}

void DBRepository::createTables()
{
    createProjectsTable();
    createTagsTable();
    createProjectsTagsTable();
    createRecordingsTable();
}

void DBRepository::createProjectsTable()
{
    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE Projects (
            project_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            recordings_directory VARCHAR(255) NOT NULL,
            project_name VARCHAR(150) NOT NULL,
            beats_per_minute INTEGER,
            musical_key VARCHAR(10),
            created_at TIMESTAMP DEFAULT (CURRENT_TIMESTAMP)
        );
    )");
    if (query.lastError().isValid()) {
        qDebug() << "Error creating Projects table: " << query.lastError();
    }
}

void DBRepository::createTagsTable()
{
    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE Tags (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE
        );
    )");
    if (query.lastError().isValid()) {
        qDebug() << "Error creating Tags table: " << query.lastError();
    }
}

void DBRepository::createProjectsTagsTable()
{
    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE projects_tags (
            project_id INTEGER,
            tag_id INTEGER,
            PRIMARY KEY (project_id, tag_id),
            FOREIGN KEY (project_id) REFERENCES Projects(project_id) ON DELETE CASCADE,
            FOREIGN KEY (tag_id) REFERENCES Tags(id) ON DELETE CASCADE
        );
    )");
    if (query.lastError().isValid()) {
        qDebug() << "Error creating projects_tags table: " << query.lastError();
    }
    QStringList tags = {"Bass", "Electric Guitar", "Rock", "Pop"};
    for (const QString &tag : tags) {
        query.prepare("INSERT OR IGNORE INTO Tags (name) VALUES (:name)");
        query.bindValue(":name", tag);

        if (!query.exec()) {
            qDebug() << "Error inserting tag:" << tag << query.lastError();
        }
    }
}

void DBRepository::createRecordingsTable()
{
    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE Recordings (
            id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            file_name VARCHAR(255) NOT NULL,
            display_name VARCHAR(255) NOT NULL,
            file_path VARCHAR(255) NOT NULL,
            project_id INT,
            recording_date DATETIME, start_time INTEGER, is_backing_track INT, duration INT,
            CONSTRAINT fk_project_id FOREIGN KEY (project_id)
                REFERENCES Projects(project_id)
        );
    )");
    if (query.lastError().isValid()) {
        qDebug() << "Error creating Recordings table: " << query.lastError();
    }
}
