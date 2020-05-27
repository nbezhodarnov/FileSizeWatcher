#ifndef FOLDERSTRATEGY_H
#define FOLDERSTRATEGY_H

#include <QString>

#include "Strategies/explorerstrategy.h"

// Конкретный класс FolderStrategy, обрабатывающий папку с группировкой по папкам
class FolderStrategy : public ExplorerStrategy
{
public:
    // конструктор
    FolderStrategy() {};

    // конкретная функция вычисления информации о размерах содержимого папки (с группировкой по элементам папки)
    // на вход подаётся QString - путь к папке, на выходе QList<FileSizeData> - список данных о размере каждого элемента папки
    QList<FileSizeData> Explore (const QString &);

private:
    // функция вычисления размера папки
    // на вход подаётся QString - путь к папке, на выходе quint64 - размер папки в байтах
    quint64 FolderSize(const QString &);
};


#endif // FOLDERSTRATEGY_H
