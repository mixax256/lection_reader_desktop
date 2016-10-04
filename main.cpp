#include <QApplication>
#include <QQmlApplicationEngine>
#include "qdatabasework.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QDataBaseWork dataBaseWork;
    dataBaseWork.createDataBase("lections");
    dataBaseWork.createTable("LECTIONS");
    return app.exec();
}
