#include "filesizedatamodel.h"


FileSizeDataModel::FileSizeDataModel(QObject *parent, QList<FileSizeData> dt) :
    QAbstractTableModel(parent)
{
    dataModel = dt;
}

//Возвращаем количество строк, в зависимости от количества данных в списке
int FileSizeDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return dataModel.count();
}

//Возвращаем количество столбцов, оно у нас постоянно
int FileSizeDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

//Возвращаем названия заголовков. Обратите внимание на тип возвращаемого значения.
QVariant FileSizeDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return section;
    }
    switch (section) {
    case NAME:
        return "Название";
    case SIZE:
        return "Размер";
    case PERCENT:
        return "В процентах";
    }
    return QVariant();
}

//Возвращаем соответствующие данные, относительно модельного индекса и роли.
//В случае, если на вход пришли не корректные данные, что может возникнуть при не
//верной реализации, также в случае, когда приходит роль (int role), которую нам не
//нужно обрабатывать, в частности Qt::DisplayRole и Qt::EditRole , возвращается пустой
//объект QVariant(). Тем самым обеспечивается устойчивая работа представления, при
//отображении модели в случае наличия ошибок.
// Модельный индекс характеризуется номером строки и столбца, следовательно в зависимости
QVariant FileSizeDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || dataModel.count() <= index.row() || (role != Qt::DisplayRole && role != Qt::EditRole))
    {
        return QVariant();
    }
    if (index.column() == 0) {
        return dataModel[index.row()].FileInfo;
    } else if (index.column() == 1) {
        QString result;
        if (dataModel[index.row()].size / (quint64)1'125'899'906'842'624) {
            result = QString::number((double)dataModel[index.row()].size / (quint64)1'125'899'906'842'624, 'f', 2) + " ТБ";
        } else if (dataModel[index.row()].size / 1073741824) {
            result = QString::number((double)dataModel[index.row()].size / 1073741824, 'f', 2) + " ГБ";
        } else if (dataModel[index.row()].size / 1048576) {
            result = QString::number((double)dataModel[index.row()].size / 1048576, 'f', 2) + " МБ";
        } else if (dataModel[index.row()].size / 1024) {
            result = QString::number((double)dataModel[index.row()].size / 1024, 'f', 2) + " КБ";
        } else {
            QString ending;
            if (((dataModel[index.row()].size % 100) / 10 == 1) || (dataModel[index.row()].size % 10 > 4)) {
                ending = " байтов";
            } else if (dataModel[index.row()].size % 10 <= 1) {
                ending = " байт";
            } else {
                ending = " байта";
            }
            result = QString::number(dataModel[index.row()].size) + ending;
        }
        return result;
    } else if (index.column() == 2) {
        QString result;
        if (dataModel[index.row()].sizePercentage > 0 && dataModel[index.row()].sizePercentage < 0.01) {
            result = "< 0.01%";
        } else {
            result = QString::number(dataModel[index.row()].sizePercentage, 'f', 2) + '%';
        }
        return result;
    }
    return QVariant();
}
