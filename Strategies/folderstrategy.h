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
    void Explore (const QString &path);

private:
    // функция вычисления размера папки
    quint64 FolderSize(const QString &path);
};


#endif // FOLDERSTRATEGY_H
