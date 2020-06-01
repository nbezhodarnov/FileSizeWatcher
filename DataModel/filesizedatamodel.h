#ifndef FILESIZEDATAMODEL_H
#define FILESIZEDATAMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QString>

#include "Strategies/FileSizeData.h"

class FileSizeDataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FileSizeDataModel(QObject *parent = nullptr, QList<FileSizeData> dt = QList<FileSizeData>());
    //Минимальный и обязательный набор необходимых методов
    //Так как нам требуется только отображать данные, то этого набора достаточно
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool hasChildren(const QModelIndex &parent) const override;
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
