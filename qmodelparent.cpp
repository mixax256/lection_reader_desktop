#include "qmodelparent.h"
#include "qdatabasework.h"


QModelParent::QModelParent(string dbName)
{
    QDataBaseWork db;
    db.createDataBase(dbName);

}

QModelParent::~QModelParent()
{

}

QVariant QModelParent::data(const QModelIndex &index, int role) const
{

}
int QModelParent::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()){
        return 0;
    }
    const DataWrapper* parentInfo = static_cast<const DataWrapper*>(parent.internalPointer());
    return parentInfo->children.size();
}

int QModelParent::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QModelIndex QModelParent::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)){
        return QModelIndex();
    }
    if (!parent.isValid()) {
        return createIndex(row, column, const_cast<DataWrapper*>(&d.children[row]));
    }
    DataWrapper* parentInfo = static_cast<DataWrapper*>(parent.internalPointer());
    return createIndex(row, column, &parentInfo->children[row]);
}

QModelIndex QModelParent::parent(const QModelIndex &child) const
{
    if (!child.parent().isValid()) {
        return QModelIndex();
    }
    DataWrapper *child_pointer = child.internalPointer();
    if (!child_pointer->parent) {
        return QModelIndex();
    }
    return createIndex(child_pointer->parent->number, 0, static_cast<void *> (child_pointer->parent));
}

