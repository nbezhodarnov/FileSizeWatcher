#include "filesizedatamodel.h"

// конструктор
FileSizeDataModel::FileSizeDataModel(QObject *parent, const QList<FileSizeData> &dt) :
    QAbstractItemModel(parent) {
    dataModel.resize(dt.size()); // задание размера массива
    auto dataIterator = dataModel.begin(); // задание итератора массива
    // занесение данных в массив
    for (auto iterator = dt.begin(); iterator != dt.end(); iterator++) {
        *dataIterator = *iterator;
        dataIterator++;
    }
}

// функция, которая возвращает количество строчек, на входе QModelIndex& - ссылка на индекс родителя, на выходе int - количество строчек
int FileSizeDataModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return dataModel.count();
}

// функция, которая возвращает количество столбцов, на входе QModelIndex& - ссылка на индекс родителя, на выходе int - количество столбцов
int FileSizeDataModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 3;
}

// функция, которая возвращает заголовок элементов модели, на входе int - секция + Qt::Orientation - ориентация + int - роль, на выходе QVariant - данные заголовка элементов модели
QVariant FileSizeDataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) { // проверка на отоброжаемость роли
        return QVariant();
    }
    if (orientation == Qt::Vertical) { // проверка на вертикальную ориентацию
        return section + 1;
    }
    switch (section) { // проверка секции
    case NAME:
        return "Название";
    case SIZE:
        return "Размер";
    case PERCENT:
        return "В процентах";
    }
    return QVariant();
}

// функция, которая возвращает элемент модели, на входе QModelIndex& - ссылка на индекс элемента и int - роль, на выходе QVariant - данные элемента модели
QVariant FileSizeDataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || dataModel.count() <= index.row() || (role != Qt::DisplayRole && role != Qt::EditRole)) { // проверка на правильность индекса
        return QVariant();
    }
    if (index.column() == 0) { // первый столбец (имя)
        return dataModel[index.row()].FileInfo;
    } else if (index.column() == 1) { // второй столбец (размер)
        QString result;
        // отображение данных в удобной размерности
        if (dataModel[index.row()].size / (quint64)1'125'899'906'842'624) { // ТБ
            result = QString::number((double)dataModel[index.row()].size / (quint64)1'125'899'906'842'624, 'f', 2) + " ТБ";
        } else if (dataModel[index.row()].size / 1073741824) { // ГБ
            result = QString::number((double)dataModel[index.row()].size / 1073741824, 'f', 2) + " ГБ";
        } else if (dataModel[index.row()].size / 1048576) { // МБ
            result = QString::number((double)dataModel[index.row()].size / 1048576, 'f', 2) + " МБ";
        } else if (dataModel[index.row()].size / 1024) { // КБ
            result = QString::number((double)dataModel[index.row()].size / 1024, 'f', 2) + " КБ";
        } else { // байты
            // отображение информации по правилам русского языка
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
    } else if (index.column() == 2) { // третий столбец (проценты)
        QString result;
        if (dataModel[index.row()].sizePercentage > 0 && dataModel[index.row()].sizePercentage < 0.01) { // проверка на малось
            result = "< 0.01%";
        } else {
            result = QString::number(dataModel[index.row()].sizePercentage, 'f', 2) + '%';
        }
        return result;
    }
    return QVariant();
}

// функция, которая позволяет изменять данные при необходимости (здесь определяется ради того, чтобы не допустить изменения данных), на входе QModelIndex& - ссылка на индекс элемента + QVariant& - ссылка на изменённые данные + int - роль, на выходе bool - флаг об успешности изменения данных
bool FileSizeDataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    // возвращаем флаг о неуспешном изменении данных
    return false;
    /*
    bool result = false;
    if (index.isValid() && role == Qt::EditRole) {
        index.row(), index.column();
        int row = index.row();
        int column = index.column();
        switch (column) {
        case 0:
            dataModel[row].FileInfo = value.toString();
            break;
        case 1:
            dataModel[row].size = value.toInt();
            break;
        case 2:
            dataModel[row].sizePercentage = value.toDouble();
            break;
        default:
            break;
        } // end switch column
        emit dataChanged(index, index);
        result = true;
    }
    return result;
    */
}

// функция, которая возвращает флаги элемента, QModelIndex& - ссылка на индекс элемента, на выходе Qt::ItemFlags - флаги
Qt::ItemFlags FileSizeDataModel::flags(const QModelIndex &index) const {
    return QAbstractItemModel::flags(index);
}

// функция, которая возвращает индекс элемента (требуется для того, чтобы класс не был абстрактным), на входе int - строка + int - столбец + QModelIndex& - ссылка на индекс родителя, на выходе QModelIndex - индекс элемента
QModelIndex FileSizeDataModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return createIndex(row, column, nullptr);
}

// функция, которая возвращает индекс родителя (требуется для того, чтобы класс не был абстрактным), на входе QModelIndex& - ссылка на индекс элемента, на выходе QModelIndex - индекс родителя
QModelIndex FileSizeDataModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

// функция, возвращающая флаг о наличии потомков (требуется для универсальности модели), на входе QModelIndex& - ссылка на индекс элемента, на выходе bool - флаг о наличии потомков
bool FileSizeDataModel::hasChildren(const QModelIndex &parent = QModelIndex()) const {
    // если это сама модель (проверка с пустым индексом), то потомки есть (список данных), иначе (сами данные) потомков нет
    return parent == QModelIndex();
}
