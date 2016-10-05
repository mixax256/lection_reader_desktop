#ifndef QMODELPARENT_H
#define QMODELPARENT_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
using namespace std;
class QModelParent : public QAbstractItemModel
{
    enum h_type {ROOT, COURSE, THEME, IMAGE};
    Q_OBJECT
public:
    QModelParent(string dbName);
    ~QModelParent();
    struct DataWrapper{
        quint16 id;
        h_type type;
        void *data;
        int number;
        DataWrapper *parent;
        QList<DataWrapper> children;
        int count;
    };

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
private:
    DataWrapper d{0, ROOT, nullptr, 0, nullptr, {}, -1};
};

#endif // QMODELPARENT_H
