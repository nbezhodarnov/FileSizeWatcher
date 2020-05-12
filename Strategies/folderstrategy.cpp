#include "folderstrategy.h"

#include <QTextStream>
#include <QFileInfo>
#include <QString>
#include <QDir>

// функция вывода информации о содержимом папки
void FolderStrategy::Explore (const QString &path)
{
    QTextStream out(stdout);
    QFileInfo pathInfo(path);

    // проверка объекта на существование
    if (pathInfo.exists() == false) {
        out << "The object doesn\'t exist.\n" << flush;
        return;
    }

    // проверка доступа к объекту
    if (pathInfo.isReadable() == false) {
        out << "The program has no access to this object.\n" << flush;
        return;
    }

    // проверка на неполноту пути
    if (QFileInfo(path + '/').isDir()) {
        pathInfo.setFile(path + '/');
    }

    if (pathInfo.isDir() && !pathInfo.isSymLink()) {
        // проверка папки на пустоту
        if (pathInfo.dir().isEmpty()) {
            out << "The folder is empty.\n" << flush;
            return;
        }

        QDir dir(path);
        quint64 totalSize = QFileInfo(path + '.').size(), tempSize; // 1 - итоговый размер папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки), 2 - временная переменная
        QList<quint64> sizes; // массив размеров объектов

        //вычисление размеров объектов
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
        {
            if (folder.isSymLink()) { // проверка на ссылку
                tempSize = folder.size();
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
            out << "The folder has size 0.\n" << flush;
            return;
        }

        //вывод результатов
        auto iterator = sizes.begin();
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
        {
            out << folder.fileName() << ", size percentage: " << ((double)*iterator / totalSize) * 100 << "%\n" << flush;
            iterator++;
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
        {
            out << file.fileName() << ", size percentage: " << ((double)*iterator / totalSize) * 100 << "%\n" << flush;
            iterator++;
        }
    } else { // обработка файла, не являющегося папкой
        out << pathInfo.fileName() << ", size percentage: 100%\n" << flush;
    }
    out << '\n' << flush;
}

// функция вычисления размера папки
quint64 FolderStrategy::FolderSize(const QString &path) {
    QDir dir(path); // текущая директория
    quint64 size = QFileInfo(path + "/.").size(); // объявление переменной, отвечающей за размер текущей папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки)

    //цикл по всем папкам в текущей папке
    foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden))
    {
        if (folder.isSymLink()) { // проверка на ссылку
            size += folder.size();
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
