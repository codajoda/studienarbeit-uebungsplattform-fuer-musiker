#ifndef TAGSERVICE_H
#define TAGSERVICE_H

#include <QtCore/qcontainerfwd.h>
class TagService
{
public:
    TagService();
    ~TagService();

    QVariantList getTags();
    QVariantList getTagsForProject(int projectId);

    bool addTag(QString tagName);
    bool attachTagToProject(int projectId, QString tagName);
    bool removeTagFromProject(int projectId, QString tagName);
};

#endif // TAGSERVICE_H
