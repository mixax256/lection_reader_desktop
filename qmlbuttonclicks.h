#ifndef QMLBUTTONCLICKS_H
#define QMLBUTTONCLICKS_H

#include <QObject>
#include "qmodelparent.h"

class qmlButtonClicks : public QObject
{
    Q_OBJECT
public:
    explicit qmlButtonClicks(QObject *parent = 0);

signals:
    void addElem(QModelIndex &parent);
    void deleteElem(QModelIndex &index);
public slots:
};

#endif // QMLBUTTONCLICKS_H
