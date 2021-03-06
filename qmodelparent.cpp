#include "qmodelparent.h"
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QPixmap>
#include <QImage>
#include <QUrl>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>

using namespace cv;

QModelParent::QModelParent(QString dbName, QString tableName)
{
    db.createDataBase(dbName, tableName);
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
    int height = image.height();
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
void QModelParent::print(QModelIndex indx){
    DataWrapper *elem = static_cast<DataWrapper *> (indx.internalPointer());
    if (elem->type == IMAGE){
        QUrl addr = (new QUrl(static_cast<IData*>(elem->data)->path))->toString();
        QPixmap pix;
        pix.load(addr.toString());
        QPrinter printer;
        QPrintDialog *dlg = new QPrintDialog(&printer,0);

        if(dlg->exec() == QDialog::Accepted) {
            QPainter painter(&printer);
            QRect rect = painter.viewport();
            QSize size = pix.size();
            if (size.width() > printer.width() && size.height() > printer.height()){
                QSize imageSize = pix.size();
                imageSize.scale((QSize(printer.width(), printer.height())), Qt::KeepAspectRatio);
                size = imageSize;
            }
            if (printer.height() > printer.width()){
                if (size.width() > printer.width()){
                    int w, h, x, y;
                    x=pix.width();
                    y=pix.height();
                    size=pix.size();
                    w=rect.width();
                    h=rect.height();
                    rect.setWidth(h);
                    rect.setHeight(w);
                    QPixmap rotatePixmap(size*2);
                    QPainter p(&rotatePixmap);
                    p.translate(rotatePixmap.size().width()/2,   rotatePixmap.size().height()/2);
                    p.rotate(90);
                    p.translate(-rotatePixmap.size().width()/2, -rotatePixmap.size().height()/2);
                    p.drawPixmap(y, x, pix);
                    pix=rotatePixmap.copy(0, y*2-x, y, x);
                    size=pix.size();
                    size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                    painter.setWindow(pix.rect());
                    painter.drawPixmap(QPoint(0, 0), pix);
                }
                else{
                        size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                        painter.setWindow(pix.rect());
                        painter.drawPixmap(QPoint(0, 0), pix);
                    }
            }
             else{
                    if (size.width() > printer.width() && size.height() > printer.height()){
                        QSize imageSize = pix.size();
                        imageSize.scale((QSize(printer.width(), printer.height())), Qt::KeepAspectRatio);
                        size = imageSize;
                    }
                    if (size.height() > printer.height()){
                        int w, h, x, y;
                        x=pix.width();
                        y=pix.height();
                        size=pix.size();
                        w=rect.width();
                        h=rect.height();
                        rect.setWidth(h);
                        rect.setHeight(w);
                        QPixmap rotatePixmap(size*2);
                        QPainter p(&rotatePixmap);
                        p.translate(rotatePixmap.size().width()/2,   rotatePixmap.size().height()/2);
                        p.rotate(90);
                        p.translate(-rotatePixmap.size().width()/2, -rotatePixmap.size().height()/2);
                        p.drawPixmap(y, x, pix);
                        pix=rotatePixmap.copy(0, y*2-x, y, x);
                        size=pix.size();
                        size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                        painter.setWindow(pix.rect());
                        painter.drawPixmap(QPoint(0, 0), pix);
                    }
                    else{
                        size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                        painter.setWindow(pix.rect());
                        painter.drawPixmap(QPoint(0, 0), pix);
                    }
                }
        }
    }
    if (elem->type == THEME && elem->children.count() > 0) {
        QPrinter printer;
        double yCoord = 0;
        double xCoord = 0;

        double lengthPage = 1000;
        QPrintDialog *dlg = new QPrintDialog(&printer,0);
        if(dlg->exec() == QDialog::Accepted){
            QPainter painter(&printer);

            QList<DataWrapper*> images = elem->children;
              for (int k = 0; k < images.count(); k++) {
                  QUrl addr = (new QUrl(static_cast<IData*>(images[k]->data)->path))->toString();
                  QPixmap pix;
                  pix.load(addr.toString());
                  bool bigImage = false;
                  QRect rect = painter.viewport();
                  QSize size = pix.size();
                  if (printer.height() > printer.width()){
                      if (size.width() > printer.width() && size.height() > printer.height()){
                          if (k)
                            printer.newPage();
                          QSize imageSize = pix.size();
                          imageSize.scale((QSize(printer.width(), printer.height())), Qt::KeepAspectRatio);
                          size = imageSize;
                          yCoord = 0;
                          bigImage = true;
                      }
                      if (( yCoord + size.height()) > lengthPage && !(size.width() > printer.width())){
                          if (!bigImage)
                            printer.newPage();
                          bigImage = false;
                         yCoord = 0;
                      }
                         if (size.width() > printer.width()){
                             if (k && !bigImage)
                               bigImage = false;;
                             bigImage = false;
                             int w, h, x, y;
                             x=pix.width();
                             y=pix.height();
                             size=pix.size();
                             w=rect.width();
                             h=rect.height();
                             rect.setWidth(h);
                             rect.setHeight(w);
                             QPixmap rotatePixmap(size*2);
                             QPainter p(&rotatePixmap);
                             p.translate(rotatePixmap.size().width()/2,   rotatePixmap.size().height()/2);
                             p.rotate(90);
                             p.translate(-rotatePixmap.size().width()/2, -rotatePixmap.size().height()/2);
                             p.drawPixmap(y, x, pix);
                             pix=rotatePixmap.copy(0, y*2-x, y, x);
                             size=pix.size();
                             size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                             painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                             painter.setWindow(pix.rect());
                             painter.drawPixmap(QPoint(0, 0), pix);
                             yCoord = 0;
                             if ((k) && (k != images.count() - 1))
                               printer.newPage();
                         }
                      else{
                          size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                          painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                          painter.setWindow(pix.rect());
                          painter.drawPixmap(QPoint(0, yCoord), pix);
                          yCoord +=  size.height();
                          bigImage = false;
                      }
                  }
                  else{
                      if (size.width() > printer.width() && size.height() > printer.height()){
                          if (k && !bigImage)
                            printer.newPage();
                          QSize imageSize = pix.size();
                          imageSize.scale((QSize(printer.width(), printer.height())), Qt::KeepAspectRatio);
                          size = imageSize;
                          xCoord = 0;
                          bigImage = true;
                      }
                      if (( xCoord + size.width()) > lengthPage && !(size.height() > printer.height()) && k){
                          if (!bigImage)
                            printer.newPage();
                          bigImage = false;
                          xCoord = 0;
                      }
                      if (size.height() > printer.height()){
                          if (k && !bigImage)
                            printer.newPage();

                          int w, h, x, y;
                          x=pix.width();
                          y=pix.height();
                          size=pix.size();
                          w=rect.width();
                          h=rect.height();
                          rect.setWidth(h);
                          rect.setHeight(w);
                          QPixmap rotatePixmap(size*2);
                          QPainter p(&rotatePixmap);
                          p.translate(rotatePixmap.size().width()/2,   rotatePixmap.size().height()/2);
                          p.rotate(90);
                          p.translate(-rotatePixmap.size().width()/2, -rotatePixmap.size().height()/2);
                          p.drawPixmap(y, x, pix);
                          pix=rotatePixmap.copy(0, y*2-x, y, x);
                          size=pix.size();
                          size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                          painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                          painter.setWindow(pix.rect());
                          painter.drawPixmap(QPoint(0, 0), pix);
                          xCoord = 0;
                          if (k && !bigImage)
                            printer.newPage();
                          bigImage = false;
                      }
                      else{
                          size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                          painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                          painter.setWindow(pix.rect());
                          painter.drawPixmap(QPoint(xCoord, 0), pix);
                          xCoord +=  size.width();
                          bigImage = false;
                      }
                  }
              }
            }
        }

    if (elem->type == COURSE && elem->children.count() > 0) {
            QPrinter printer;
            double yCoord = 0;
            double xCoord = 0;

            double lengthPage = 1000;
            QPrintDialog *dlg = new QPrintDialog(&printer,0);
            if(dlg->exec() == QDialog::Accepted){
            QPainter painter(&printer);
            QList<DataWrapper*> themes = elem->children;
            for (int k = 0; k < themes.count(); k++) {
                if (themes[k]->children.count() == 0) {
                    fetchMore(index(k, 0, indx));
                }
                QList<DataWrapper*> images = themes[k]->children;
                for (int l = 0; l < images.count(); l++) {
                    QUrl addr = (new QUrl(static_cast<IData*>(images[l]->data)->path))->toString();
                    QPixmap pix;
                    pix.load(addr.toString());
                    QRect rect = painter.viewport();
                    QSize size = pix.size();\
                    bool bigImage = false;
                    if (printer.height() > printer.width()){
                        if (size.width() > printer.width() && size.height() > printer.height()){
                            if (k || l)
                              printer.newPage();
                            QSize imageSize = pix.size();
                            imageSize.scale((QSize(printer.width(), printer.height())), Qt::KeepAspectRatio);
                            size = imageSize;
                            bigImage = true;
                            yCoord = 0;
                        }
                        if (( yCoord + size.height()) > lengthPage && !(size.width() > printer.width())){
                            if (!bigImage)
                              printer.newPage();
                            bigImage = false;
                           yCoord = 0;
                        }
                           if (size.width() > printer.width()){
                               if ((l || k) && !bigImage )
                                 printer.newPage();
                               int w, h, x, y;
                               bigImage = false;
                               x=pix.width();
                               y=pix.height();
                               size=pix.size();
                               w=rect.width();
                               h=rect.height();
                               rect.setWidth(h);
                               rect.setHeight(w);
                               QPixmap rotatePixmap(size*2);
                               QPainter p(&rotatePixmap);
                               p.translate(rotatePixmap.size().width()/2,   rotatePixmap.size().height()/2);
                               p.rotate(90);
                               p.translate(-rotatePixmap.size().width()/2, -rotatePixmap.size().height()/2);
                               p.drawPixmap(y, x, pix);
                               pix=rotatePixmap.copy(0, y*2-x, y, x);
                               size=pix.size();
                               size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                               painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                               painter.setWindow(pix.rect());
                               painter.drawPixmap(QPoint(0, 0), pix);
                               yCoord = 0;
                               if (((l) && (l != (images.count() - 1) ) && k != (themes.count() - 1)) || (1 == images.count() && ((l || k) || ((l != (images.count() - 1) ) && k != (themes.count() - 1)))))
                                 printer.newPage();
                        }
                        else{
                            size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                            painter.setWindow(pix.rect());
                            painter.drawPixmap(QPoint(0, yCoord), pix);
                            yCoord +=  size.height();
                            bigImage = false;
                        }
                    }
                    else{
                        if (size.width() > printer.width() && size.height() > printer.height()){
                            if (k || l)
                              printer.newPage();
                            QSize imageSize = pix.size();
                            imageSize.scale((QSize(printer.width(), printer.height())), Qt::KeepAspectRatio);
                            size = imageSize;
                            bigImage = true;
                            xCoord = 0;

                        }
                        if (( xCoord + size.width() > lengthPage) && !(size.height() > printer.height())) {
                            if (!bigImage)
                              printer.newPage();
                            bigImage = false;
                            xCoord = 0;
                        }
                        if (size.height() > printer.height()){
                            if ((l || k) && !bigImage)
                              printer.newPage();
                            bigImage = false;
                            int w, h, x, y;
                            x=pix.width();
                            y=pix.height();
                            size=pix.size();
                            w=rect.width();
                            h=rect.height();
                            rect.setWidth(h);
                            rect.setHeight(w);
                            QPixmap rotatePixmap(size*2);
                            QPainter p(&rotatePixmap);
                            p.translate(rotatePixmap.size().width()/2,   rotatePixmap.size().height()/2);
                            p.rotate(90);
                            p.translate(-rotatePixmap.size().width()/2, -rotatePixmap.size().height()/2);
                            p.drawPixmap(y, x, pix);
                            pix=rotatePixmap.copy(0, y*2-x, y, x);
                            size=pix.size();
                            size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                            painter.setWindow(pix.rect());
                            painter.drawPixmap(QPoint(0, 0), pix);
                            xCoord = 0;
                            if (((l) && (l != (images.count() - 1) ) && k != (themes.count() - 1)) || (1 == images.count() && ((l || k) || ((l != (images.count() - 1) ) && k != (themes.count() - 1)))))
                              printer.newPage();
                        }
                        else{
                            size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
                            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                            painter.setWindow(pix.rect());
                            painter.drawPixmap(QPoint(xCoord, 0), pix);
                            xCoord +=  size.width();
                            bigImage = false;
                        }
                    }
                 }
            }
          }
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
    case TYPE_MODEL:
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
    case TYPE_MODEL:
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
        QString path;
        // удаление локальных изображений (в зависимости от типа текущего элемента)
        if (data_parent->children[row]->type == IMAGE) {
            path = static_cast<IData*>(data_parent->children[row]->data)->path;
            QUrl url = QUrl(path);
            QDirIterator it(path.mid(0, path.lastIndexOf('/')),
                            QStringList() << url.fileName().mid(0, url.fileName().lastIndexOf('.')).append('*').append(path.mid(path.lastIndexOf('.'))),
                            QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                QFile::remove(it.next());
            }
        }

        if (data_parent->children[row]->type == THEME) {
            for (int j = 0; j < data_parent->children[row]->children.count(); j++) {
                QList<DataWrapper*> images = data_parent->children[row]->children;
                for (int k = 0; k < images.count(); k++) {
                    path = static_cast<IData*>(images[k]->data)->path;
                    QUrl url = QUrl(path);
                    QDirIterator it(path.mid(0, path.lastIndexOf('/')),
                                    QStringList() << url.fileName().mid(0, url.fileName().lastIndexOf('.')).append('*').append(path.mid(path.lastIndexOf('.'))),
                                    QDir::Files, QDirIterator::Subdirectories);
                    while (it.hasNext()) {
                        QFile::remove(it.next());
                    }
                }
            }
        }

        if (data_parent->children[row]->type == COURSE) {
            for (int j = 0; j < data_parent->children[row]->children.count(); j++) {
                QList<DataWrapper*> themes = data_parent->children[row]->children;
                for (int k = 0; k < themes.count(); k++) {
                    QList<DataWrapper*> images = themes[k]->children;
                    for (int l = 0; l < images.count(); l++) {
                        path = static_cast<IData*>(images[l]->data)->path;
                        QUrl url = QUrl(path);
                        QDirIterator it(path.mid(0, path.lastIndexOf('/')),
                                        QStringList() << url.fileName().mid(0, url.fileName().lastIndexOf('.')).append('*').append(path.mid(path.lastIndexOf('.'))),
                                        QDir::Files, QDirIterator::Subdirectories);
                        while (it.hasNext()) {
                            QFile::remove(it.next());
                        }
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
                setData(child, COURSE, TYPE_MODEL);
            break;
            case COURSE:
                setData(child, THEME, TYPE_MODEL);
            break;
            case THEME:
                setData(child, IMAGE, TYPE_MODEL);
            break;
            default:
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
            if (QFile::exists(newFileName)) {  // нужно для случая, когда удаляли предпоследний файл, а потом снова добавили
                newFileName = newFileName.mid(0, newFileName.lastIndexOf('_') + 1)
                               .append(QString::number(data->count)).append(fileType);
            }
            QFile::copy(oldFile, newFileName);
            setData(child, newFileName, PATH);
        }
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

QUrl QModelParent::imageImprovment(QUrl image)
{
    Mat src = imread(image.path().toStdString(), 1); // исходное изображение
    Mat dst;
    src.convertTo(src, CV_32FC1,1.0/255.0); // преобразование в 32 битное изображение
    cvtColor(src, src, CV_BGR2GRAY);        // перевод в оттенки серого
    GaussianBlur(src, dst, Size(51,51), 0, 0);  // Гаусово размытие
    src = src / dst;                        // делим исходное изображение на Гаусово размытие, получаем нужный результат
    src.convertTo(src, CV_16U, 255);        // преобразование в 16 битное изображение (для сохранения)
    QString path = image.path();
    path = path.mid(0, path.lastIndexOf('.'))
               .append("imp")
               .append(path.mid(path.lastIndexOf('.')));
    imwrite(path.toStdString(), src);
    return QUrl::fromLocalFile(path);
}

QUrl QModelParent::cutImage(QUrl image, int x, int y, int width, int height, int showedWidth, int showedHeight)
{
    IplImage* img = cvLoadImage(image.path().toStdString().c_str(), 1);
    double propWidth = (double) img->width / showedWidth;
    double propHeight = (double) img->height / showedHeight;
    cvSetImageROI(img, cvRect(x * propWidth, y * propHeight, width * propWidth, height * propHeight));
    IplImage* subImg = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
    cvCopy(img, subImg, NULL);
    cvResetImageROI(img);
    QString path = image.path();
    path = path.mid(0, path.lastIndexOf('.'))
               .append("cut")
               .append(path.mid(path.lastIndexOf('.')));
    cvSaveImage(path.toStdString().c_str(), subImg);
    return QUrl::fromLocalFile(path);
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

void QModelParent::saveChanges(QUrl lastImage, int rotationAngle, QUrl originalImage)
{
    IplImage *src = 0;
    src = cvLoadImage(lastImage.path().toStdString().c_str(), 1);
    if (rotationAngle) {
        IplImage *dst = cvCloneImage(src);
        CvMat* rotMat = cvCreateMat(2, 3, CV_32FC1);
        CvPoint2D32f center = cvPoint2D32f(src->width/2, src->height/2);
        cv2DRotationMatrix(center, -rotationAngle, 0.7, rotMat); // -угол -- для поворота по часовой стрелке
        cvWarpAffine(src, dst, rotMat);
        cvCopy(dst, src);
    }

    QString path = originalImage.path();
    cvSaveImage(path.toStdString().c_str(), src);

    QDirIterator it(path.mid(0, path.lastIndexOf('/')),
                    QStringList() << originalImage.fileName().mid(0, originalImage.fileName().lastIndexOf('.')).append("?*").append(path.mid(path.lastIndexOf('.'))),
                    QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile::remove(it.next());
    }
}

void QModelParent::cancelChanges(QUrl originalImage)
{
    QString path = originalImage.path();
    QDirIterator it(path.mid(0, path.lastIndexOf('/')),
                    QStringList() << originalImage.fileName().mid(0, originalImage.fileName().lastIndexOf('.')).append("?*").append(path.mid(path.lastIndexOf('.'))),
                    QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile::remove(it.next());
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
