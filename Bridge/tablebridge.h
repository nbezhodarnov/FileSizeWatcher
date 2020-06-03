#ifndef TABLEBRIDGE_H
#define TABLEBRIDGE_H

#include <QAbstractItemModel>
#include <QTableView>

#include "DataModel/filesizedatamodel.h"
#include "Bridge/abstractbridge.h"

// Конкретный класс, служащий для отображения информации в виде таблицы
class TableBridge : public AbstractBridge
{
public:
    // конструктор
    TableBridge(QObject *p = nullptr);
    // конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде таблицы
    QWidget* UpdateData(const QList<FileSizeData>&);
    // деструктор
    ~TableBridge();
private:
    QAbstractItemView *view; // указатель на виджет, отображающий информацию в виде таблицы
    QAbstractItemModel *model; // указатель на модель, из которой строится таблица
};

#endif // TABLEBRIDGE_H
