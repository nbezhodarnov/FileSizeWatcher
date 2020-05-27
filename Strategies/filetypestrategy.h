#ifndef FILETYPESTRATEGY_H
#define FILETYPESTRATEGY_H

#include <QFileInfo>
#include <QString>

#include "Strategies/explorerstrategy.h"

// Конкретный класс FileTypeStrategy, обрабатывающий папку с группировкой по типам
class FileTypeStrategy : public ExplorerStrategy
{
public:
    // конструктор
    FileTypeStrategy() {};

    // конкретная функция вычисления информации о размерах содержимого папки (с группировкой по типам данных)
    // на вход подаётся QString - путь к папке, на выходе QList<FileSizeData> - список данных о размере, занимаемом каждым типом в папке
    QList<FileSizeData> Explore (const QString &);

private:
    // функция вычисления размера вложенной папки
    // на вход подаётся QString - путь к папке, ссылка на QHash<QString, quint64> - объект, содержащий информацию о размере, занимаемом каждым типом в папке (QString - тип данных, quint64 - размер в байтах)
    void FolderSize(const QString &, QHash<QString, quint64> &);

    // функция определения типа файла
    // на вход подаётся QFileInfo - информация о файле, на выходе QString - тип файла
    QString FileType(const QFileInfo &);
};

#endif // FILETYPESTRATEGY_H
