#ifndef DBREPOSITORY_H
#define DBREPOSITORY_H

#include <QtSql/qsqldatabase.h>

class DBRepository
{

public:
    static DBRepository& instance();

    bool openDatabase();
    void closeDatabase();
    bool executeQuery(const QString &queryStr);

    void createTables();
    void createProjectsTable();
    void createTagsTable();
    void createProjectsTagsTable();
    void createRecordingsTable();
private:
    DBRepository();
    ~DBRepository();

    QSqlDatabase db;
};

#endif // DBREPOSITORY_H
