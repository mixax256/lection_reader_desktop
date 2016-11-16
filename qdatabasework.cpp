#include <QtSql>
#include "qdatabasework.h"

QDataBaseWork::QDataBaseWork()
{

}

QDataBaseWork::~QDataBaseWork(){
    this->dbase.close();
}

void QDataBaseWork::createDataBase(string dbName) {
    this->dbase = QSqlDatabase::addDatabase("QSQLITE");
    this->dbase.setDatabaseName(dbName.c_str());
    if (!this->dbase.open()) {
        qDebug() << this->dbase.lastError().text();
    }
}

void QDataBaseWork::createTable(string tableName) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("CREATE TABLE ").append(tableName.c_str()).append(" (")
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
        error.append("Table ").append(tableName.c_str()).append(" not created");
        qDebug() << error;
    }
}

int QDataBaseWork::insertInTable(string tableName, int pid, string path, string comment, string tag, int type, int number) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("INSERT into ").append(tableName.c_str())
         .append(" (pid, path, comments, tags, type, number)")
         .append("values(:pid, :path, :comment, :tag, :type, :number);");
    sqlQuery.prepare(query);
    sqlQuery.bindValue(":pid", pid);
    sqlQuery.bindValue(":path", path.c_str());
    sqlQuery.bindValue(":comment", comment.c_str());
    sqlQuery.bindValue(":tag", tag.c_str());
    sqlQuery.bindValue(":type", type);
    sqlQuery.bindValue(":number", number);
    bool exec = sqlQuery.exec();
    if (!exec) {
        QString error;
        error.append("Row in ").append(tableName.c_str()).append(" not inserted");
        qDebug() << error;
    }
    query.append("SELECT max(id) as id from ").append(tableName.c_str());
    exec = sqlQuery.exec();
    return sqlQuery.value("id");
}

void QDataBaseWork::updateInTable(string tableName, map<string, string> rowsNamesAndValues, string whereCondition) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("UPDATE ").append(tableName.c_str()).append("\nSET ");
    for (auto it = rowsNamesAndValues.begin(); it != rowsNamesAndValues.end(); ++it) {
        if (it != rowsNamesAndValues.begin()) {
            query.append(",\n");
        }
        query.append((*it).first.c_str()).append(" = ").append((*it).second.c_str());
    }

    if (!whereCondition.empty())
        query.append("\nWHERE ").append(whereCondition.c_str());

    query.append(";");
    bool exec = sqlQuery.exec(query);
    if (!exec) {
        QString error;
        error.append("Table ").append(tableName.c_str()).append(" not updated");
        qDebug() << error;
    }
}

void QDataBaseWork::deleteFromTable(string tableName, string whereCondition) {
    QSqlQuery sqlQuery;
    QString query;
    query.append("DELETE from ").append(tableName.c_str());
    if (!whereCondition.empty())
         query.append("\nWHERE ").append(whereCondition.c_str());
    bool exec = sqlQuery.exec(query);
    if (!exec) {
        QString error;
        if (!whereCondition.empty())
            error.append("Row in ");
        else
            error.append("Rows in");
        error.append(tableName.c_str()).append(" not deleted");
        qDebug() << error;
    }
}
