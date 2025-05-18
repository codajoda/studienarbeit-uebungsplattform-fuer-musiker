#ifndef TAG_H
#define TAG_H

#include <QtCore/qobject.h>

class Tag : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
public:
    explicit Tag(int id, const QString &name, QObject *parent = nullptr) : QObject(parent), m_id(id), m_name(name) {}

    int id() const { return m_id; }
    QString name() const { return m_name; }


private:
    int m_id;
    QString m_name;
};

#endif // TAG_H
