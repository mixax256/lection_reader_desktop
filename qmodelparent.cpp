#include "qmodelparent.h"
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QPixmap>
#include <QImage>
#include <QUrl>
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>



QModelParent::QModelParent(QString dbName, QString tableName)
{
    db.createDataBase(dbName);
    this->tableName = tableName;
    fetchAll (QModelIndex());
}
QUrl QModelParent::toBlack(QUrl data)
{
    QString path;
    int begin = data.toString().lastIndexOf(":") + 1;
    path = data.toString().mid(begin);
    QImage converted, image;
    image.load(path);
    converted = image;
    QRgb col;
    int gray;
    int width  = image.width();
    int height = image.height();\
    for (int i = 0; i < width; ++i)
      {
          for (int j = 0; j < height; ++j)
          {
              col = image.pixel(i, j);
              gray = qGray(col);
              converted.setPixel(i, j, qRgb(gray, gray, gray));
          }
      }
    converted = converted.convertToFormat(QImage::Format_RGB32).
            convertToFormat(QImage::Format_Indexed8);
    path = path.mid(0, path.indexOf(".")) + "_grey" + path.mid(path.indexOf("."));
    converted.save(path);
    QString beginFile = data.toString().mid(0, begin);
    QUrl newUrl = QUrl::fromUserInput(beginFile + path);
    return newUrl;
}
void QModelParent::print(QUrl data)

{
    QPixmap pix;
    pix.load(data.toLocalFile());
    QPrinter printer;
          QPrintDialog *dlg = new QPrintDialog(&printer,0);
          if(dlg->exec() == QDialog::Accepted) {
                  QPainter painter(&printer);
                  painter.drawPixmap(QPoint(0, 0), pix);
                  painter.end();
          }


}
QModelIndex QModelParent::sibling(int row, int column, const QModelIndex &idx) const
{
    const DataWrapper *data;
    if (!hasIndex(row,column,idx.parent())) return {};
    if (!idx.parent().isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (idx.parent().internalPointer());
    }
    return createIndex(row,column,data->children[row]);
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
                         return (new QUrl(static_cast<IData*> (elem->data)->path))->fileName();
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
    bool status = false;
    DataWrapper *data = static_cast<DataWrapper *> (index.internalPointer());
    // вытягиваем все данные из данного элемента
    int pid;
    QString path;
    QString comment;
    QStringList tags;
    QString tag;
    int type = data->type;
    int number = data->number;

    // если не в первый раз заполняем
    if (data->id != NULL) {
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
    }

    // собираем условие на апдейт (т.к. у нас за один раз можно поставить только одно значение,
    // то мы будем по кускам заполнять апдейт)
    map<QString, QString> rowsNamesAndValues;
    QString whereCondition;
    whereCondition.append("id = ").append(QString::number(data->id));
    switch (role) {
    case PID:
        pid = value.toInt();
        rowsNamesAndValues["pid"] = QString::number(pid);
        break;
    case PATH:
        path = value.toString();
        rowsNamesAndValues["path"] = (new QString("'"))->append(path).append("'");
        break;
    case COMMENT:
        comment = value.toString();
        rowsNamesAndValues["comment"] = (new QString("'"))->append(comment).append("'");
        break;
    case TAG:
        tag = value.toString();
        rowsNamesAndValues["tag"] = (new QString("'"))->append(tag).append("'");
         break;
    case TYPE:
        type = value.toInt();
        rowsNamesAndValues["type"] = QString::number(type);
        break;
    case NUMBER:
        number = value.toInt();
        rowsNamesAndValues["number"] = QString::number(number);
        break;
    }
    // всё пусто, делаем setData впервые
    if (data->id == NULL) {
        int id = db.insertInTable(tableName, pid, path, comment, tag, type, number);
        // сразу определяем id в таблице и указываем тип, чтобы в дальнейшем
        // вытянуть нужные данные
        data->id = id;
        if ( type != IMAGE ) {
            data->data = new HData;
        }
        else {
            data->data = new IData;
        }
        status = true;
    }
    else { // иначе докидываем новые данные
            status = db.updateInTable(tableName, rowsNamesAndValues, whereCondition);
    }
    switch (role) {
    case PID:
        break;
    case PATH:
        if ( data->type != IMAGE ) {
            static_cast<HData*>(data->data)->name = path;
        }
        else {
            static_cast<IData*>(data->data)->path = path;
        }
        break;
    case COMMENT:
        if ( data->type != IMAGE ) {
            static_cast<HData*>(data->data)->comments = comment;
        }
        else {
            static_cast<IData*>(data->data)->comments = comment;
        }
        break;
    case TAG:
        if ( data->type != IMAGE ) {
            static_cast<HData*>(data->data)->tags = tags;
        }
        else {
            static_cast<IData*>(data->data)->tags = tags;
        }
         break;
    case TYPE:
        data->type = h_type(type);
        break;
    case NUMBER:
        data->number = number;
        break;
    }
    return status;
}

bool QModelParent::insertRows(int row, int count, const QModelIndex &parent)
{
    QModelIndex parentIndex;
    if (!parent.isValid()) {
        parentIndex = QModelIndex();
    }
    else {
        parentIndex = parent;
    }
    beginInsertRows(parentIndex, row, row + count - 1);
    DataWrapper *data;
    if (!parent.isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (parent.internalPointer());
    }
    for (int i = 0; i < count; i++) {
        data->children.insert(row, new DataWrapper() );
    }
    data->count = data->count + count;
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

        // удаление локальных изображений (в зависимости от типа текущего элемента)
        if (data_parent->children[row]->type == IMAGE) {
            QFile::remove(static_cast<IData*>(data_parent->children[row]->data)->path);
        }
        if (data_parent->children[row]->type == THEME) {
            for (int j = 0; j < data_parent->children[row]->children.count(); j++) {
                QList<DataWrapper*> images = data_parent->children[row]->children;
                for (int k = 0; k < images.count(); k++) {
                    QFile::remove(static_cast<IData*>(images[k]->data)->path);
                }
            }
        }

        if (data_parent->children[row]->type == COURSE) {
            for (int j = 0; j < data_parent->children[row]->children.count(); j++) {
                QList<DataWrapper*> themes = data_parent->children[row]->children;
                for (int k = 0; k < themes.count(); k++) {
                    QList<DataWrapper*> images = themes[k]->children;
                    for (int l = 0; l < images.count(); l++) {
                        QFile::remove(static_cast<IData*>(images[l]->data)->path);
                    }
                }
            }
        }
        // удаление детей из таблицы
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

bool QModelParent::addItem(QString name, QModelIndex parent)
{
    DataWrapper *data;
    if (!parent.isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (parent.internalPointer());
    }
    if (data->type != IMAGE) {
        insertRow(data->count, parent);
        QModelIndex child = index(data->count - 1, 0, parent);
        switch (data->type) {
            case ROOT:
                setData(child, COURSE, TYPE);
            break;
            case COURSE:
                setData(child, THEME, TYPE);
            break;
            case THEME:
                setData(child, IMAGE, TYPE);
            break;
        }
        setData(child, data->id, PID);
        if (data->type != THEME) {
            setData(child, name, PATH);
        }
        else {
            QUrl url = QUrl(name);
            QString oldFile = url.path();
            //костыль для работы с виндовс
            if (!QFile::exists(oldFile)){
                oldFile = oldFile.mid(oldFile.indexOf('/')+1);
            }
            int indx = oldFile.lastIndexOf('.');
            QString fileType = oldFile.mid(indx);
            QString newFileName = QDir::homePath().append(QDir::separator())
                    .append(new QString(DEFAULT_PATH)).append(QDir::separator())
                    .append(static_cast<HData*>(data->parent->data)->name)
                    .append('_').append(static_cast<HData*>(data->data)->name)
                    .append('_').append(QString::number(data->count-1))
                    .append(fileType);
            QFile::copy(oldFile, newFileName);
            setData(child, newFileName, PATH);
        }
        //setData(child, "", COMMENT);
        //setData(child, "", TAG);
        setData(child, data->count-1, NUMBER);
        DataWrapper* data_child = static_cast<DataWrapper *> (child.internalPointer());
        data_child->parent = data;
    }
    return true;
}

int QModelParent::getType(QModelIndex index)
{
    DataWrapper* data;
    if (!index.isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (index.internalPointer());
    }
    return (int)data->type;
}

QModelIndex QModelParent::getImage(QModelIndex curIndex, int pressedKey)
{
    QModelIndex parent = curIndex.parent();
    DataWrapper* data = static_cast<DataWrapper *> (parent.internalPointer());
    if ( (curIndex.row() == 0 && pressedKey == Qt::Key_Left)
         || (curIndex.row() == data->count - 1 && pressedKey == Qt::Key_Right) ){
        return curIndex;
    }
    else {
        if (pressedKey == Qt::Key_Left) {
            return index(curIndex.row() - 1, 0, parent);
        }
        else {
            return index(curIndex.row() + 1, 0, parent);
        }
    }
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
