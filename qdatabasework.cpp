#include <QtSql>
#include "qdatabasework.h"

QDataBaseWork::QDataBaseWork()
{

}

QDataBaseWork::~QDataBaseWork(){
    this->dbase.close();
}

void QDataBaseWork::createDataBase(QString dbName) {
    this->dbase = QSqlDatabase::addDatabase("QSQLITE");
    this->dbase.setDatabaseName(dbName);
    if (!this->dbase.open()) {
        qDebug() << this->dbase.lastError().text();
    }
}

void QDataBaseWork::createTable(QString tableName) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("CREATE TABLE ").append(tableName).append(" (")
         .append("id         integer primary key autoincrement,"
                 "pid        integer,"
                 "path       text,"
                 "comments   text,"
                 "tags       text,"
                 "type       integer,"
                 "number     integer);");

    bool exec = sqlQuery.exec(query);
    if (!exec) {
        QString error;
        error.append("Table ").append(tableName).append(" not created");
        qDebug() << error;
    }
}

int QDataBaseWork::insertInTable(QString tableName, int pid, QString path, QString comment, QString tag, int type, int number) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("INSERT into ").append(tableName)
         .append(" (pid, path, comments, tags, type, number)")
         .append("values(:pid, :path, :comment, :tag, :type, :number);");
    sqlQuery.prepare(query);
    sqlQuery.bindValue(":pid", pid);
    sqlQuery.bindValue(":path", path);
    sqlQuery.bindValue(":comment", comment);
    sqlQuery.bindValue(":tag", tag);
    sqlQuery.bindValue(":type", type);
    sqlQuery.bindValue(":number", number);
    bool exec = sqlQuery.exec();
    if (!exec) {
        QString error;
        error.append("Row in ").append(tableName).append(" not inserted");
        qDebug() << error;
    }
    query.append("SELECT max(id) as id from ").append(tableName);
    exec = sqlQuery.exec();
    return sqlQuery.value("id").toInt();
}

void QDataBaseWork::updateInTable(QString tableName, map<QString, QString> rowsNamesAndValues, QString whereCondition) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("UPDATE ").append(tableName).append("\nSET ");
    for (auto it = rowsNamesAndValues.begin(); it != rowsNamesAndValues.end(); ++it) {
        if (it != rowsNamesAndValues.begin()) {
            query.append(",\n");
        }
        query.append((*it).first).append(" = ").append((*it).second);
    }

    if (!whereCondition.isEmpty())
        query.append("\nWHERE ").append(whereCondition);

    query.append(";");
    bool exec = sqlQuery.exec(query);
    if (!exec) {
        QString error;
        error.append("Table ").append(tableName).append(" not updated");
        qDebug() << error;
    }
}

bool QDataBaseWork::deleteFromTable(QString tableName, QString whereCondition) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("DELETE from ").append(tableName);
    if (!whereCondition.isEmpty())
         query.append("\nWHERE ").append(whereCondition);
    bool exec = sqlQuery.exec(query);
    if (!exec) {
        QString error;
        if (!whereCondition.isEmpty())
            error.append("Row in ");
        else
            error.append("Rows in");
        error.append(tableName).append(" not deleted");
        qDebug() << error;
    }
    return exec;
}
