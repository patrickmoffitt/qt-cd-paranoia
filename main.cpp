#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QIcon>
#include "qtcdparanoia.hpp"

QtCdParanoia qtCdParanoia;

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/images/CD-icon.png"));

    // Load and apply an application stylesheet
    QFile styleFile(":/stylesheet.css");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    app.setStyleSheet(style);

    QQmlApplicationEngine engine;

    QtCdParanoia q;
    engine.rootContext()->setContextProperty("QtCdParanoia", &q);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
