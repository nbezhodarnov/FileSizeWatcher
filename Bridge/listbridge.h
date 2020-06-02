#ifndef LISTBRIDGE_H
#define LISTBRIDGE_H

#include <QAbstractItemModel>
#include <QTreeView>

#include "DataModel/filesizedatamodel.h"
#include "Bridge/abstractbridge.h"

class ListBridge : public AbstractBridge
{
public:
    ListBridge(QObject *p = nullptr);
    QWidget* UpdateData(QList<FileSizeData>);
    ~ListBridge();
private:
    QAbstractItemView *view;
    QAbstractItemModel *model;
};

#endif // LISTBRIDGE_H
