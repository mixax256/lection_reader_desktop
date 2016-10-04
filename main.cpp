#include <QApplication>
#include <QQmlApplicationEngine>
#include "qdatabasework.h"

int main(int argc, char *argv[])
{
    string dbName = "lections";
    string tableName = "LECTIONS";
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QDataBaseWork dataBaseWork;
    dataBaseWork.createDataBase(dbName);
    //dataBaseWork.createTable(tableName);
    //dataBaseWork.insertInTable(tableName, NULL, "/home", "тестовая запись", "", 1);

    //map <string, string> rowsAndVals = {{"tags", "148"}};
    //dataBaseWork.updateInTable(tableName, rowsAndVals, "id = 2");
    dataBaseWork.deleteFromTable(tableName, "");
    return app.exec();
}
