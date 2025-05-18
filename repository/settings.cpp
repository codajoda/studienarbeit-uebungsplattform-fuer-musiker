#include "settings.h"

Settings& Settings::instance() {
    static Settings instance;
    return instance;
}

Settings::Settings()
    : settings("MyCompany", "MusicaApp") {}

QVariant Settings::getValue(const QString &key, const QVariant &defaultValue) const {
    return settings.value(key, defaultValue);
}

void Settings::setValue(const QString &key, const QVariant &value) {
    settings.setValue(key, value);
}

void Settings::deleteValue(const QString &key)
{
    settings.remove(key);
}
