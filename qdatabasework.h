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
    void createDataBase(QString dbName);
    void createTable(QString tableName);
    int insertInTable(QString tableName, int pid, QString path, QString commet, QString tag, int type, int number);
    void updateInTable(QString tableName, map<QString, QString> rowsNamesAndValues, QString whereCondition);
    void deleteFromTable(QString tableName, QString whereCondition);
};

#endif // QDATABASEWORK_H
