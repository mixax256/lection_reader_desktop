#include <QtSql>
#include "qdatabasework.h"

QDataBaseWork::QDataBaseWork()
{

}

QDataBaseWork::~QDataBaseWork(){
    this->dbase.close();
}

void QDataBaseWork::createDataBase(char* dbName) {
    this->dbase = QSqlDatabase::addDatabase("QSQLITE");
    this->dbase.setDatabaseName(dbName);
    if (!this->dbase.open()) {
        qDebug() << this->dbase.lastError().text();
    }
}

void QDataBaseWork::createTable(char *tableName) {
    QSqlQuery a_query;
    QString query;
    query.append("CREATE TABLE ").append(tableName).append(" (")
         .append("id         integer primary key autoincrement,"
                 "pid        integer,"
                 "path       text,"
                 "comments   text,"
                 "tags       text,"
                 "type       integer );");
                    ;
    bool exec = a_query.exec(query);
    if (!exec) {
        QString error;
        error.append("Table ").append(tableName).append(" not created");
        qDebug() << error;
    }
}
