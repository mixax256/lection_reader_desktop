#include <QApplication>
#include <QQmlApplicationEngine>
#include <QHBoxLayout>
#include <QObject>
#include <QTableView>
#include <QTreeView>
#include "qdatabasework.h"
#include "qmodelparent.h"
#include <QQmlContext>
using namespace std;

int main(int argc, char *argv[])
{
    QString dbName = "lections";
    QString tableName = "LECTIONS";
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QModelParent model(dbName, tableName);
    engine.rootContext()->setContextProperty("modelTree", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //QDataBaseWork dataBaseWork;
    //dataBaseWork.createDataBase(dbName);
    //dataBaseWork.createTable(tableName);
    //dataBaseWork.insertInTable(tableName, NULL, "/home", "тестовая запись", "", 1);

    //map <QString, QString> rowsAndVals = {{"tags", "148"}};
    //dataBaseWork.updateInTable(tableName, rowsAndVals, "id = 2");
    /*dataBaseWork.deleteFromTable(tableName, "");*/


    return app.exec();
}
