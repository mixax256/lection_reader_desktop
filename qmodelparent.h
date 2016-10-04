#ifndef QMODELPARENT_H
#define QMODELPARENT_H

#include <QObject>

class QModelParent : public QObject
{
    Q_OBJECT
public:
    explicit QModelParent(QObject *parent = 0);

signals:

public slots:
};

#endif // QMODELPARENT_H