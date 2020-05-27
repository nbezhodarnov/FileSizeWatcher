#include "folderstrategy.h"

#include <QTextStream>
#include <QFileInfo>
#include <QString>
#include <QFile>
#include <QDir>

// конкретная функция вычисления информации о размерах содержимого папки (с группировкой по элементам папки)
// на вход подаётся QString - путь к папке, на выходе QList<FileSizeData> - список данных о размере каждого элемента папки
QList<FileSizeData> FolderStrategy::Explore (const QString &path)
{
    QFileInfo pathInfo(path);
    QTextStream out(stdout);
    QList<FileSizeData> result; // список данных о размере каждого элемента папки

    // проверка объекта на существование
    if (pathInfo.exists() == false) {
        out << "The object doesn\'t exist.\n" << flush;
        return QList<FileSizeData>();
    }

    // проверка доступа к объекту
    if (pathInfo.isReadable() == false) {
        out << "The program has no access to this object.\n" << flush;
        return QList<FileSizeData>();
    }

    // проверка на неполноту пути
    if (QFileInfo(path + '/').isDir()) {
        pathInfo.setFile(path + '/');
    }

    // проверка на то, что на входе была подана папка
    if (pathInfo.isDir() && !pathInfo.isSymLink()) {
        // проверка папки на пустоту
        if (pathInfo.dir().isEmpty()) {
            out << "The folder is empty.\n" << flush;
            return QList<FileSizeData>();
        }

        QDir dir(pathInfo.absoluteFilePath());
        #if defined(Q_OS_WIN)
        quint64 totalSize = 0, tempSize; // 1 - итоговый размер папки, 2 - временная переменная
        #else
        quint64 totalSize = QFileInfo(pathInfo.absoluteFilePath() + '.').size(), tempSize; // 1 - итоговый размер папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки), 2 - временная переменная
        #endif
        QList<quint64> sizes; // массив размеров объектов

        //вычисление размеров объектов
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Name))
        {
            if (folder.isSymLink()) { // проверка на ссылку
                if (folder.fileName().mid(folder.fileName().lastIndexOf('.') + 1) == "lnk") { // проверка на ярлык
                    continue; // пропускаем ярлыки
                } else {
                    tempSize = 0; // символические ссылки ничего не весят
                }
            } else {
                tempSize = FolderSize(folder.path() + '/' + folder.fileName()); // вычисляется размер папки
            }
            sizes.append(tempSize);
            totalSize += tempSize;
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Name))
        {
            if (file.isSymLink()) { // проверка на ярлык
                QFile fileOpen(file.absoluteFilePath());
                fileOpen.open(QIODevice::ReadOnly); // в силу работы компилятора для вычисления размера ярлыка необходимо открыть его
                tempSize = fileOpen.size(); // вычисляется размер файла
                fileOpen.close();
            } else {
                tempSize = file.size(); // вычисляется размер файла
            }
            sizes.append(tempSize);
            totalSize += tempSize;
        }

        // если папка ничего не весит, то выходим из функции
        if (totalSize == 0) {
            out << "The folder has size 0.\n" << flush;
            return QList<FileSizeData>();
        }

        //вывод результатов
        auto iterator = sizes.begin();
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Name))
        {
            if (folder.isSymLink() && folder.fileName().mid(folder.fileName().lastIndexOf('.') + 1) == "lnk") { // проверка на ярлык
                continue; // пропускаем ярлыки
            }
            result.append(FileSizeData(folder.fileName(), *iterator, ((double)*iterator / totalSize) * 100));
            iterator++;
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Name))
        {
            result.append(FileSizeData(file.fileName(), *iterator, ((double)*iterator / totalSize) * 100));
            iterator++;
        }
    } else { // обработка файла, не являющегося папкой
        quint64 fileSize = pathInfo.size(); // вычисляется размер файла
        #if defined(Q_OS_WIN)
        if (pathInfo.isSymLink()) { // проверка на ярлык
            QFile fileOpen(pathInfo.absoluteFilePath());
            fileOpen.open(QIODevice::ReadOnly); // в силу работы компилятора для вычисления размера ярлыка необходимо открыть его
            fileSize = fileOpen.size(); // вычисляется размер файла
            fileOpen.close();
        }
        #endif
        result.append(FileSizeData(pathInfo.fileName(), fileSize, 100));
    }
    return result;
}

// функция вычисления размера папки
// на вход подаётся QString - путь к папке, на выходе quint64 - размер папки в байтах
quint64 FolderStrategy::FolderSize(const QString &path) {
    QDir dir(path); // текущая директория
    #if defined(Q_OS_WIN)
    quint64 size = 0; // объявление переменной, отвечающей за размер текущей папки
    #else
    quint64 size = QFileInfo(path + "/.").size(); // объявление переменной, отвечающей за размер текущей папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки)
    #endif
    //цикл по всем папкам в текущей папке
    foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System | QDir::NoSymLinks))
    {
        size += FolderSize(folder.path() + '/' + folder.fileName()); // вычисляется размер папки
    }

    //цикл по всем файлам в папке
    foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
    {
        if (file.isSymLink()) { // проверка на ярлык
            QFile fileOpen(file.absoluteFilePath());
            fileOpen.open(QIODevice::ReadOnly); // в силу работы компилятора для вычисления размера ярлыка необходимо открыть его
            size += fileOpen.size(); // вычисляется размер файла
            fileOpen.close();
            continue;
        }
        size += file.size(); // вычисляется размер файла
    }

    return size; // получаем итоговый размер
}
