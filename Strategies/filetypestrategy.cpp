#include "filetypestrategy.h"

#include <QTextStream>
#include <QFileInfo>
#include <QString>
#include <QDir>

//функция вывода информации о содержимом папки
void FileTypeStrategy::Explore (const QString &path)
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

    if (pathInfo.isDir() && !pathInfo.isSymLink()) {
        // подготовка строки к работе
        QString pathAddition(".");
        if (path.at(path.size() - 1) != '/') {
            pathAddition.insert(0, '/');
        }

        QDir dir(path);
        QHash<QString, quint64> hash;
        quint64 temp = QFileInfo(path + pathAddition).size();
        if (temp) {
            hash[FileType(path + pathAddition)] = temp;
        }

        //вычисление размеров объектов
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden))
        {
            if (folder.isSymLink()) { // проверка на ссылку
                hash[FileType(folder)] = folder.size();
            } else {
                FolderSize(folder.path() + '/' + folder.fileName(), hash); // проводятся вычисления с папкой
            }
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden))
        {
            hash[FileType(file)] = file.size(); // вычисляется размер файла
        }

        QStringList types; // массив типов
        quint64 totalSize = 0; // итоговый размер папки

        // вычисление итогового размера папки и заполнение массива типов
        for (auto iterator = hash.begin(); iterator != hash.end(); iterator++) {
            totalSize += *iterator;
            types.append(iterator.key());
        }

        // если папка ничего не весит, то выходим из функции
        if (totalSize == 0) {
            out << "The folder has size 0.\n" << flush;
            return;
        }

        types.sort(); // сортировка типов по их названиям

        //вывод результатов
        for (int i = 0; i < types.size(); i++) {
            out << types[i] << ", size percentage: " << ((double)hash[types[i]] / totalSize) * 100 << "%\n" << flush;
        }
    } else { // обработка файла, не являющегося папокй
        out << FileType(pathInfo) << ", size percentage: 100%\n" << flush;
    }
    out << '\n' << flush;
}

// функция обработки вложенной папки
void FileTypeStrategy::FolderSize(const QString &path, QHash<QString, quint64> &hash) {
    QDir dir(path); // текущая директория
    quint64 temp = QFileInfo(path + "/.").size();
    if (temp) {
        hash[FileType(QFileInfo(path + "/."))] += temp;
    }
    //цикл по всем папкам в текущей папке
    foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden))
    {
        if (folder.isSymLink()) { // проверка на ссылку
            hash[FileType(folder)] += folder.size();
        } else {
            FolderSize(folder.path() + '/' + folder.fileName(), hash); // проводятся вычисления с вложенной папкой
        }
    }

    //цикл по всем файлам в папке
    foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden))
    {
        hash[FileType(file)] += file.size(); // вычисляется размер файла
    }
}

// функция определения типа файла
QString FileTypeStrategy::FileType(const QFileInfo &file) {
    if (file.isSymbolicLink()) { // ссылка
        return "symlink";
    }
    if (file.isShortcut()) { // ярлык
        return "shortcut";
    }
    if (file.isDir()) { // папка
        return "directory";
    }

    QString fileName = file.fileName(); // название файла
    int i = fileName.size() - 1;
    for (; (i >= 0) && (fileName[i] != '.'); i--); // поиск символа .

    if (i < 0) { // неизвестный тип (отсутствует символ .)
        return "unknown";
    }

    return fileName.mid(i);//возвращение типа файла
}
