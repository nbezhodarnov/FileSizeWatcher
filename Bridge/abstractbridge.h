#ifndef ABSTRACTBRIDGE_H
#define ABSTRACTBRIDGE_H

#include <QWidget>
#include <QList>

#include "Strategies/FileSizeData.h"

class AbstractBridge
{
public:
    AbstractBridge(QObject *p = nullptr): parent(p) {};
    virtual QWidget* UpdateData(QList<FileSizeData>) = 0;
    virtual ~AbstractBridge() {};
protected:
    QObject *parent;
};

#endif // ABSTRACTBRIDGE_H
