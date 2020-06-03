#ifndef LISTBRIDGE_H
#define LISTBRIDGE_H

#include <QAbstractItemModel>
#include <QTreeView>

#include "DataModel/filesizedatamodel.h"
#include "Bridge/abstractbridge.h"

// Конкретный класс, служащий для отображения информации в виде списка
class ListBridge : public AbstractBridge
{
public:
    // конструктор
    ListBridge(QObject *p = nullptr);
    // конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде списка
    QWidget* UpdateData(const QList<FileSizeData>&);
    // деструктор
    ~ListBridge();
private:
    QAbstractItemView *view; // указатель на виджет, отображающий информацию в виде списка
    QAbstractItemModel *model; // указатель на модель, из которой строится список
};

#endif // LISTBRIDGE_H
