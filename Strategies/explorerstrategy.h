#ifndef EXPLORERSTRATEGY_H
#define EXPLORERSTRATEGY_H

#include <QString>

// Абстрактный класс ExplorerStrategy, используемый для определения стратегий
class ExplorerStrategy
{
public:
    ExplorerStrategy() {};
    virtual QString Explore (const QString &)=0;
    virtual ~ExplorerStrategy() {};
};

#endif // EXPLORERSTRATEGY_H
