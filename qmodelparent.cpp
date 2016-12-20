#include "qmodelparent.h"
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QPixmap>
#include <QImage>

QModelParent::QModelParent(QString dbName, QString tableName)
{
    db.createDataBase(dbName);
    this->tableName = tableName;
    fetchAll (QModelIndex());
}
void QModelParent::print(QVariant data)

{


    QImage img = qvariant_cast<QImage>(data);
    QPrinter printer;
          QPrintDialog *dlg = new QPrintDialog(&printer,0);
          if(dlg->exec() == QDialog::Accepted) {
                  QPainter painter(&printer);
                  painter.drawImage(QPoint(0,0),img);
                  painter.end();
          }


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
                 int pos;
                 QString path;
                 switch (elem->type) {
                     case COURSE:
                     case THEME:
                         return static_cast<HData*> (elem->data)->name;
                     case IMAGE:
                         path = static_cast<IData*> (elem->data)->path;
                         pos = path.lastIndexOf('/') + 1;
                         return path.mid(pos, path.lastIndexOf('.') - pos);
                     default:
                         break;
                 }
             }
         }
        if (role == Qt::DecorationRole||Qt::SizeHintRole){
             if (index.isValid()) {
                 const DataWrapper *elem = static_cast<DataWrapper *> (index.internalPointer());
                     if (elem->type == IMAGE){
                         if (role == Qt::DecorationRole){
                             return QUrl::fromLocalFile(static_cast<IData*> (elem->data)->path);
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
        return parent_pointer->children.count();
    }
    const DataWrapper* parentInfo = static_cast<const DataWrapper*>(parent.internalPointer());
    return parentInfo->children.count();
}

int QModelParent::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QModelIndex QModelParent::index(int row, int column, const QModelIndex &parent) const
{
    const DataWrapper* parentInfo;
    if (!parent.isValid()) {
        parentInfo = &d;
    }
    else {
        parentInfo = static_cast<DataWrapper*>(parent.internalPointer());
    }
    return createIndex(row, column, parentInfo->children[row]);
}

QModelIndex QModelParent::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }
    DataWrapper *child_pointer = static_cast<DataWrapper *> (child.internalPointer());
    if (!child_pointer->parent) {
        return QModelIndex();
    }
    return createIndex(child_pointer->parent->number, 0, static_cast<void *> (child_pointer->parent));
}
void QModelParent::fetchAll (const QModelIndex &parent)
{
    DataWrapper *data;
    if (!parent.isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (parent.internalPointer());
    }
    QSqlQuery query;

    data->count = getChildrenCount(data->type, data->id);
    query.prepare ("SELECT * from " + tableName + " where pid = :id order by number");
    query.bindValue (":id", data->id);
    query.exec();
    beginInsertRows(parent, 0, data->count-1);
    while (query.next()) {
        auto id = query.value ("id").toUInt();
        auto comment = query.value ("comments").toString();
        auto number = query.value ("number").toInt();
        QStringList tags = query.value ("tags").toStringList();
        switch (data->type) {
        case ROOT:
        case COURSE: {
            auto type = query.value ("type").toInt();
            auto name = query.value ("path").toString();
            data->children.append (
                        new DataWrapper{id, (h_type)type,
                                        new HData{name, comment, tags},
                                        number, data, {}, getChildrenCount (h_type(type),id)});
            break;
        }
        case THEME: {
            auto path = query.value ("path").toString();
            data->children.append (
                        new DataWrapper{id, IMAGE,
                                        new IData{path, comment, tags},
                                        number, data, {}, getChildrenCount (IMAGE,id)});
            break;
        }
        default:
            break;
        }
    }
    endInsertRows();
}

bool QModelParent::setData(const QModelIndex &index, const QVariant &value, int role)
{
    DataWrapper *data = static_cast<DataWrapper *> (index.internalPointer());
    // вытягиваем все данные из данного элемента
    int pid = data->parent->id;
    QString path;
    QString comment;
    QStringList tags;
    QString tag;
    int type = data->type;
    int number = data->number;

    switch (data->type) {
        case ROOT:
        case COURSE:
        case THEME:
        {
            path = static_cast<HData*>(data->data)->name;
            comment = static_cast<HData*>(data->data)->comments;

            tags = static_cast<HData*>(data->data)->tags;
            break;
        }
        case IMAGE: {
            path = static_cast<IData*>(data->data)->path;
            comment = static_cast<IData*>(data->data)->comments;
            tags = static_cast<IData*>(data->data)->tags;
            break;
        }
    }
    for (auto it = tags.begin(); it != tags.end(); it++) {
        tag = tag.append(*it).append(';');
    }

    // собираем условие на апдейт (т.к. у нас за один раз можно поставить только одно значение,
    // то мы будем по кускам заполнять апдейт)
    map<QString, QString> rowsNamesAndValues;
    QString whereCondition = "id = " + data->id;
    switch (role) {
    case PID:
        pid = value.toInt();
        rowsNamesAndValues["pid"] = "" + pid;
        break;
    case PATH:
        path = value.toString();
        rowsNamesAndValues["path"] = path;
        break;
    case COMMENT:
        comment = value.toString();
        rowsNamesAndValues["comment"] = comment;
        break;
    case TAG:
        tag = value.toString();
        rowsNamesAndValues["tag"] = tag;
         break;
    case TYPE:
        type = value.toInt();
        rowsNamesAndValues["type"] = "" + type;
        break;
    case NUMBER:
        number = value.toInt();
        rowsNamesAndValues["number"] = "" + number;
        break;
    }
    // всё пусто, делаем setData впервые
    if (data->id == NULL) {
        int id = db.insertInTable(tableName, pid, path, comment, tag, type, number);
        data->id = id;
        return true;
    }
    else { // иначе докидываем новые данные
            db.updateInTable(tableName, rowsNamesAndValues, whereCondition);
            return true;
    }
    return false;
}

bool QModelParent::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    DataWrapper *data = static_cast<DataWrapper *> (parent.internalPointer());
    for (int i = 0; i < count; i++) {
        data->children.insert(row + i, new DataWrapper() );
    }

    endInsertRows();
    return true;
}

bool QModelParent::removeRows(int row, int count, const QModelIndex &parent)
{
    bool status = false;
    beginRemoveRows(parent, row, row + count - 1);
    DataWrapper *data_parent = static_cast<DataWrapper *> (parent.internalPointer());
    int id;
    QString whereCondition;
    for (int i = 0; i < count; i++) {
        // вытягиваем id дочернего элемента
        id = data_parent->children[row]->id;

        // удаляем его дочерние элементы
        whereCondition.append("pid = ").append(QString::number(id));
        status = db.deleteFromTable(tableName, whereCondition);

        // удаляем его из базы
        whereCondition = (new QString("id = "))->append(QString::number(id));
        status = db.deleteFromTable(tableName, whereCondition);

        // удаляем из списка детей
        data_parent->children.removeAt(row);
    }
    data_parent->count = getChildrenCount(data_parent->type, data_parent->id);
    endRemoveRows();
    return status;
}

bool QModelParent::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    DataWrapper *oldParent = static_cast<DataWrapper *> (sourceParent.internalPointer());
    DataWrapper *newParent = static_cast<DataWrapper *> (destinationParent.internalPointer());

    int id;
    map<QString, QString> rowsAndVals;
    for (int i = 0; i < count; i++) {
        // перекидываем строку из старого родителя в новый
        newParent->children.insert(destinationChild + i, oldParent->children.at(sourceRow));
        // меняем родителя в базе
        id = oldParent->children.at(sourceRow)->id;
        rowsAndVals["pid"] = newParent->id;
        db.updateInTable(tableName, rowsAndVals, (new QString("id = "))->append(QString::number(id)));
        //удаляем строку из старого родителя
        removeRow(sourceRow, sourceParent);
    }
    return true;
}

bool QModelParent::hasChildren(const QModelIndex &parent) const
{
    const DataWrapper *parentInfo;
    if (!parent.isValid()) {
        parentInfo = &d;
    }
    else {
        parentInfo = static_cast<DataWrapper *> (parent.internalPointer());
    }
    return parentInfo->count != 0;
}

bool QModelParent::deleteItem(int row, QModelIndex index)
{
    QModelIndex parent = index.parent();
    bool status = false;
    DataWrapper *data = static_cast<DataWrapper *> (parent.internalPointer());
    if (parent.isValid() && data->count > 0) {
        status = removeRow(row, parent);

        if (data != &d && data->count == 0) {
            fetchMore(parent);
        }

        if (data == &d) {
            beginResetModel();
            endResetModel();
        }
    }
    return status;
}

int QModelParent::getChildrenCount (h_type type, quint16 pid) const
{
    QSqlQuery query;
    switch (type) {
    case ROOT:
    case COURSE:
    case THEME:
        query.prepare ("SELECT COUNT (*) from " + tableName + " where pid = :id ");
        break;
    case IMAGE:
        return 0;
    default:
        break;
    }
    query.bindValue (":id", pid);
    query.exec();
    query.next();

    int count = query.value (0).toInt();
    return count;
}
void QModelParent::fetchMore (const QModelIndex &parent)
{
    fetchAll (parent);
}
bool QModelParent::canFetchMore (const QModelIndex &parent) const
{
    const DataWrapper *data;
    if (!parent.isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (parent.internalPointer());
    }

    return data->children.size() < data->count;
}
