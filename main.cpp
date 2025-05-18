#include "audiocontroller.h"
#include "dbrepository.h"
#include "dbcontroller.h"
#include "juce_events/juce_events.h"
#include "settings.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QStandardPaths>
#include <QtCore/qfileinfo.h>
#include <QDir>

int main(int argc, char *argv[])
{
    juce::ScopedJuceInitialiser_GUI juceInit;
    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));
    qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", QByteArray("Light"));

    QApplication app(argc, argv);

    qDebug() << QApplication::applicationDirPath();

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<DBController>("com.example", 1, 0, "DBController", [](QQmlEngine*, QJSEngine*) -> QObject* {
        return new DBController();
    });
    qmlRegisterSingletonType<AudioController>("com.example", 1, 0, "AudioController", [](QQmlEngine*, QJSEngine*) -> QObject* {
        return new AudioController();
    });

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);
    Settings::instance().setValue("databasePath", dataDir + "/musica.db");
    DBRepository::instance();

    engine.loadFromModule("Musica", "Main");

    return app.exec();
}
