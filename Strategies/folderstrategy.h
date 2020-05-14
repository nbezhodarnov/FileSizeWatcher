#ifndef FOLDERSTRATEGY_H
#define FOLDERSTRATEGY_H

#include <QString>

#include "Strategies/explorerstrategy.h"

// Конкретный класс FolderStrategy, обрабатывающий папку с группировкой по папкам
class FolderStrategy : public ExplorerStrategy
{
public:
    FolderStrategy() {};

    // функция вывода информации о содержимом папки
    QString Explore (const QString &);

private:
    // функция вычисления размера папки
    quint64 FolderSize(const QString &);
};


#endif // FOLDERSTRATEGY_H
