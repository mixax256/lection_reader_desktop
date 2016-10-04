#ifndef QDATABASEWORK_H
#define QDATABASEWORK_H

#include <QtSql>

class QDataBaseWork
{
private:
    QSqlDatabase dbase;
public:
    QDataBaseWork();
    ~QDataBaseWork();
    void createDataBase(char* dbName);
    void createTable(char* tableName);
};

#endif // QDATABASEWORK_H
