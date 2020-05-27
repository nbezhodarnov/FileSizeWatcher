#ifndef EXPLORERSTRATEGY_H
#define EXPLORERSTRATEGY_H

#include <QString>
#include <QList>

#include "FileSizeData.h"

// Абстрактный класс ExplorerStrategy, используемый для определения стратегий
class ExplorerStrategy
{
public:
    // конструктор
    ExplorerStrategy() {};

    // абстрактная функция вычисления информации о размерах содержимого папки
    // на вход подаётся QString - путь к папке, на выходе QList<FileSizeData> - список данных о размере каждого элемента папки
    virtual QList<FileSizeData> Explore (const QString &) = 0;

    // виртуальный деструктор
    virtual ~ExplorerStrategy() {};
};

#endif // EXPLORERSTRATEGY_H
