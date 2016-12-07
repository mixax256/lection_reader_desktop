#include <QApplication>
#include <QQmlApplicationEngine>
#include <QHBoxLayout>
#include <QObject>
#include <QTableView>
#include <QTreeView>
#include "qdatabasework.h"
#include "qmodelparent.h"
using namespace std;

int main(int argc, char *argv[])
{
    QString dbName = "lections";
    QString tableName = "LECTIONS";
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //QDataBaseWork dataBaseWork;
    //dataBaseWork.createDataBase(dbName);
    //dataBaseWork.createTable(tableName);
    //dataBaseWork.insertInTable(tableName, NULL, "/home", "тестовая запись", "", 1);

    //map <QString, QString> rowsAndVals = {{"tags", "148"}};
    //dataBaseWork.updateInTable(tableName, rowsAndVals, "id = 2");
    /*dataBaseWork.deleteFromTable(tableName, "");*/

    QModelParent model(dbName, tableName);

    /*QModelParent model(dbName);
>>>>>>> design
    QWidget widget;
     QHBoxLayout layout (&widget);
     QTreeView view;
     QTableView lview;
     layout.addWidget(&view);
     //layout.addWidget(&lview);
         view.setModel(&model);
         //lview.setModel(&model);
       //  lview.setRootIndex(ip->index(0,0,ip->index(0,0,QModelIndex())));
    // QObject::connect (&view,SIGNAL(clicked(QModelIndex)),&lview,SLOT(setRootIndex(QModelIndex)));
    // QObject::connect (&view,SIGNAL(entered(QModelIndex)),&lview,SLOT(setRootIndex(QModelIndex)));
    // QObject::connect (&view,SIGNAL(activated(QModelIndex)),&lview,SLOT(setRootIndex(QModelIndex)));
     widget.show();*/
    return app.exec();
}
