#ifndef FILETYPESTRATEGY_H
#define FILETYPESTRATEGY_H

#include <QFileInfo>
#include <QString>

#include "Strategies/explorerstrategy.h"

// Конкретный класс FileTypeStrategy, обрабатывающий папку с группировкой по типам
class FileTypeStrategy : public ExplorerStrategy
{
public:
    FileTypeStrategy() {};

    //функция вывода информации о содержимом папки
    void Explore (const QString &);

private:
    // функция обработки вложенной папки
    void FolderSize(const QString &, QHash<QString, quint64> &);

    // функция определения типа файла
    QString FileType(const QFileInfo &);
};

#endif // FILETYPESTRATEGY_H
