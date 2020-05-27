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

    //абстрактная функция вычисления
    virtual QList<FileSizeData> Explore (const QString &) = 0;

    // виртуальный деструктор
    virtual ~ExplorerStrategy() {};
};

#endif // EXPLORERSTRATEGY_H
