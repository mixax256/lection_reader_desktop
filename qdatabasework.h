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
    void createDataBase(string dbName);
    void createTable(string tableName);
    void insertInTable(string tableName, int pid, string path, string commet, string tag, int type);
    void updateInTable(string tableName, map<string, string> rowsNamesAndValues, string whereCondition);
    void deleteFromTable(string tableName, string whereCondition);
};

#endif // QDATABASEWORK_H
