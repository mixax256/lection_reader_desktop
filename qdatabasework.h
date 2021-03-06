#ifndef QDATABASEWORK_H
#define QDATABASEWORK_H

#include <QtSql>
#include <map>

using namespace std;

class QDataBaseWork
{
private:
    QSqlDatabase dbase;
public:
    QDataBaseWork();
    ~QDataBaseWork();
    void createDataBase(QString dbName, QString tableName);
    void createTable(QString tableName);
    int insertInTable(QString tableName, int pid, QString path, QString commet, QString tag, int type, int number);
    bool updateInTable(QString tableName, map<QString, QString> rowsNamesAndValues, QString whereCondition);
    bool deleteFromTable(QString tableName, QString whereCondition);
};

#endif // QDATABASEWORK_H
