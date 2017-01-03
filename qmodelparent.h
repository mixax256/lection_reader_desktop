#ifndef QMODELPARENT_H
#define QMODELPARENT_H

#define PID           1111111
#define PATH          1111112
#define COMMENT       1111113
#define TAG           1111114
#define TYPE          1111115
#define NUMBER        1111116
#define DEFAULT_PATH  "LectionReader"

#include "qdatabasework.h"
#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
using namespace std;
class QModelParent : public QAbstractItemModel
{
    enum h_type {ROOT, COURSE, THEME, IMAGE};
    Q_OBJECT
public:
    QModelParent(QString dbName, QString tableName);
    ~QModelParent();
    struct DataWrapper{
        quint16 id;
        h_type type;
        void *data;
        int number;
        DataWrapper *parent;
        QList<DataWrapper*> children;
        int count;
    };
    struct IData {
        QString path;
        QString comments;
        QStringList tags;
    };

    struct HData {
        QString name;
        QString comments;
        QStringList tags;
    };

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

private:
    DataWrapper d{0, ROOT, nullptr, 0, nullptr, {}, 0};
    void fetchMore (const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;
    int getChildrenCount (h_type type, quint16 pid) const;
    void fetchAll (const QModelIndex &parent);
    QDataBaseWork db;
    QString tableName;

    // QAbstractItemModel interface
public:
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);
    bool hasChildren(const QModelIndex &parent) const;
    Q_INVOKABLE bool deleteItem(int row, QModelIndex index);
    Q_INVOKABLE void print(QUrl data);
    Q_INVOKABLE void toBlack(QUrl data);
    Q_INVOKABLE bool addItem(QString name, QModelIndex parent);
    Q_INVOKABLE int getType(QModelIndex index);
};

#endif // QMODELPARENT_H
