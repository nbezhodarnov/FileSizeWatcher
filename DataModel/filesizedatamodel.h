#ifndef FILESIZEDATAMODEL_H
#define FILESIZEDATAMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QString>

#include "Strategies/FileSizeData.h"

class FileSizeDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    FileSizeDataModel(QObject *parent = nullptr, QList<FileSizeData> dt = QList<FileSizeData>());
    //Минимальный и обязательный набор необходимых методов
    //Так как нам требуется только отображать данные, то этого набора достаточно
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    //Определим перечисление NameColumn для индексации столбцов нашей таблицы.
    enum NameColumn {
        NAME,
        SIZE,
        PERCENT
    };
    //Представим данные модели в виде списка.
    QList<FileSizeData> dataModel;
};

#endif
