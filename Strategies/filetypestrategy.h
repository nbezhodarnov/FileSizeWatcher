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
    void Explore (const QString &path);

private:
    // функция обработки вложенной папки
    void FolderSize(const QString &path, QHash<QString, quint64> &hash);

    // функция определения типа файла
    QString FileType(const QFileInfo &file);
};

#endif // FILETYPESTRATEGY_H
