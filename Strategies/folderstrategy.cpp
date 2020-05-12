#include "folderstrategy.h"

#include <QFileInfo>
#include <QString>
#include <QDir>

// функция вывода информации о содержимом папки
QString FolderStrategy::Explore (const QString &path)
{
    QFileInfo pathInfo(path);
    QString result;

    // проверка объекта на существование
    if (pathInfo.exists() == false) {
        result += "The object doesn\'t exist.\n";
        return result;
    }

    // проверка доступа к объекту
    if (pathInfo.isReadable() == false) {
        result += "The program has no access to this object.\n";
        return result;
    }

    // проверка на неполноту пути
    if (QFileInfo(path + '/').isDir()) {
        pathInfo.setFile(path + '/');
    }

    if (pathInfo.isDir() && !pathInfo.isSymLink()) {
        // проверка папки на пустоту
        if (pathInfo.dir().isEmpty()) {
            result += "The folder is empty.\n";
            return result;
        }

        QDir dir(pathInfo.absoluteFilePath());
        quint64 totalSize = QFileInfo(pathInfo.absoluteFilePath() + '.').size(), tempSize; // 1 - итоговый размер папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки), 2 - временная переменная
        QList<quint64> sizes; // массив размеров объектов

        //вычисление размеров объектов
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
        {
            if (folder.isSymLink()) { // проверка на ссылку
                if (folder.isShortcut()) {
                    tempSize = folder.size();
                } else {
                    tempSize = 0;
                }
            } else {
                tempSize = FolderSize(folder.path() + '/' + folder.fileName()); // вычисляется размер папки
            }
            sizes.append(tempSize);
            totalSize += tempSize;
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
        {
            tempSize = file.size(); // вычисляется размер файла
            sizes.append(tempSize);
            totalSize += tempSize;
        }

        // если папка ничего не весит, то выходим из функции
        if (totalSize == 0) {
            result += "The folder has size 0.\n";
            return result;
        }

        //вывод результатов
        auto iterator = sizes.begin();
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
        {
            result += folder.fileName() + ", size percentage: " + QString::number(((double)*iterator / totalSize) * 100) + "%\n";
            iterator++;
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
        {
            result += file.fileName() + ", size percentage: " + QString::number(((double)*iterator / totalSize) * 100) + "%\n";
            iterator++;
        }
    } else { // обработка файла, не являющегося папкой
        result += pathInfo.fileName() + ", size percentage: 100%\n";
    }
    return result;
}

// функция вычисления размера папки
quint64 FolderStrategy::FolderSize(const QString &path) {
    QDir dir(path); // текущая директория
    quint64 size = QFileInfo(path + "/.").size(); // объявление переменной, отвечающей за размер текущей папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки)

    //цикл по всем папкам в текущей папке
    foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden))
    {
        if (folder.isSymLink()) { // проверка на ссылку
            if (folder.isShortcut()) {
                size += folder.size();
            }
        } else {
            size += FolderSize(folder.path() + '/' + folder.fileName()); // вычисляется размер папки
        }
    }

    //цикл по всем файлам в папке
    foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden))
    {
        size += file.size(); // вычисляется размер файла
    }

    return size; // получаем итоговый размер
}
