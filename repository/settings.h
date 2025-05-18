#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings {
public:
    static Settings& instance();

    QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void deleteValue(const QString &key);

private:
    Settings();
    QSettings settings;
};

#endif // SETTINGS_H
