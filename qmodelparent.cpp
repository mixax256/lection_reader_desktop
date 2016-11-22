#include "qmodelparent.h"


QModelParent::QModelParent(QString dbName1)
{
    db.createDataBase(dbName1);
    dbname = dbName1;
    fetchAll (QModelIndex());
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
//        if (role == Qt::DecorationRole||Qt::SizeHintRole){
//             if (index.isValid()) {
//                 const DataWrapper *elem = static_cast<DataWrapper *> (index.internalPointer());
//                     if (elem->type == IMAGE){
//                         QPixmap pix;
//                         pix.load(static_cast<IData*> (elem->data)->path);
//                         if (role == Qt::DecorationRole){
//                             return pix;
//                         }
//                         else {
//                             return pix.size()/4;
//                         }
//                 }
//             }
//         }
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
    Q_UNUSED(parent);
    return 1;
}

QModelIndex QModelParent::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)){
        return QModelIndex();
    }
    if (!parent.isValid()) {
        return createIndex(row, column, d.children[row]);
    }
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
    beginInsertRows(parent, 0, data->children.size());
    data->children.clear();
    QSqlQuery query;
    query.prepare ("SELECT * from LECTIONS where pid = :id order by number");
    query.bindValue (":id", data->id);
    query.exec();
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
    data->count = data->children.size();
    endInsertRows();
}

bool QModelParent::setData(const QModelIndex &index, const QVariant &value, int role)
{
    DataWrapper *data = static_cast<DataWrapper *> (index.internalPointer());
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
    if (data->id == NULL) {
        int id = db.insertInTable(dbname, pid, path, comment, tag, type, number);
        data->id = id;
        return true;
    }
    else {
            db.updateInTable(dbname, rowsNamesAndValues, whereCondition);
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
    return true;
}

bool QModelParent::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    return true;
}
int QModelParent::getChildrenCount (h_type type, quint16 pid) const
{
    QSqlQuery query;
    switch (type) {
    case ROOT:
    case COURSE:
    case THEME:
        query.prepare ("SELECT COUNT (*) from LECTIONS where pid = :id ");
        break;
    case IMAGE:
        return 0;
    default:
        break;
    }
    query.bindValue (":id", pid);
    query.exec();
    query.next();
    qDebug() << query.executedQuery();
    qDebug() << query.lastError();
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
