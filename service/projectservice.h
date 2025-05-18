#ifndef PROJECTSERVICE_H
#define PROJECTSERVICE_H

#include "Project.h"
#include <QString>
#include <QVector>

class ProjectService {
public:
    ProjectService();
    ~ProjectService();

    QVariantList getAllProjects();
    Project* getProjectById(int projectId);
    QString getProjectFolderAbsolutePath(int projectId);

    int createProject(const QString project_name, const int beats_per_minute, const QString musical_key, const QString recordings_directory);
    bool deleteProject(int projectId);
    bool refreshAllProjects();
};

#endif // PROJECTSERVICE_H
