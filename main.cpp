#include <QApplication>
#include <QQmlApplicationEngine>
#include <QHBoxLayout>
#include <QObject>
#include <QTableView>
#include <QTreeView>
#include <QDir>
#include "qdatabasework.h"
#include "qmodelparent.h"
#include <QQmlContext>

#include <QTextCodec>
using namespace std;

int main(int argc, char *argv[])
{
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QString dbName = "lections";
    QString tableName = "LECTIONS";
    QApplication app(argc, argv);

    QDir defaultDir(QDir::homePath());
    defaultDir.mkdir(DEFAULT_PATH);
    QQmlApplicationEngine engine;
    QModelParent model(dbName, tableName);
    engine.rootContext()->setContextProperty("modelTree", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
