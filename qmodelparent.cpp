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
    if (!index.isValid()) return {};
        if (role == Qt::DisplayRole) {
             if (index.isValid()) {
                 if (!index.isValid()) {
                     return &d;
                 }
                 const DataWrapper *elem = static_cast<DataWrapper *> (index.internalPointer());
                 switch (elem->type) {
                     case COURSE:
                     case THEME:
                         return static_cast<HData*> (elem->data)->name;
                     case IMAGE:
                         return static_cast<IData*> (elem->data)->path;
                     default:
                         break;
                 }
             }
         }
        if (role == Qt::DecorationRole||Qt::SizeHintRole){
             if (index.isValid()) {
                 const DataWrapper *elem = static_cast<DataWrapper *> (index.internalPointer());
                     if (elem->type == IMAGE){
                         QPixmap pix;
                         pix.load(static_cast<IData*> (elem->data)->path);
                         if (role == Qt::DecorationRole){
                             return pix;
                         }
                         else {
                             return pix.size()/2;
                         }
                 }
             }
         }
        return QVariant();
}
int QModelParent::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()){
        const DataWrapper *parent_pointer = &d;
        return parent_pointer->count;
    }
    const DataWrapper* parentInfo = static_cast<const DataWrapper*>(parent.internalPointer());
    return parentInfo->count;
}

int QModelParent::columnCount(const QModelIndex &parent) const
{
    return 1;
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
    DataWrapper *child_pointer = static_cast<DataWrapper *> (child.internalPointer());
    if (!child_pointer->parent) {
        return QModelIndex();
    }
    return createIndex(child_pointer->parent->number, 0, static_cast<void *> (child_pointer->parent));
}

