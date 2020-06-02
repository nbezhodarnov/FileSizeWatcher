#ifndef TABLEBRIDGE_H
#define TABLEBRIDGE_H

#include <QAbstractItemModel>
#include <QTableView>

#include "DataModel/filesizedatamodel.h"
#include "Bridge/abstractbridge.h"

class TableBridge : public AbstractBridge
{
public:
    TableBridge(QObject *p = nullptr);
    QWidget* UpdateData(QList<FileSizeData>);
    ~TableBridge();
private:
    QAbstractItemView *view;
    QAbstractItemModel *model;
};

#endif // TABLEBRIDGE_H
