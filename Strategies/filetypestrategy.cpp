#include "filetypestrategy.h"

#include <QFileInfo>
#include <QString>
#include <QDir>

//функция вывода информации о содержимом папки
QString FileTypeStrategy::Explore (const QString &path)
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
        QHash<QString, quint64> hash;
        quint64 temp = QFileInfo(pathInfo.absoluteFilePath() + '.').size();
        if (temp) {
            hash[FileType(pathInfo.absoluteFilePath() + '.')] = temp;
        }

        //вычисление размеров объектов
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
        {
            if (folder.isSymLink()) { // проверка на ссылку
                if (FileType(folder) == ".lnk") {
                    hash[FileType(folder)] += folder.size();
                }
            } else {
                FolderSize(folder.path() + '/' + folder.fileName(), hash); // проводятся вычисления с папкой
            }
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
        {
            hash[FileType(file)] += file.size(); // вычисляется размер файла
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
            result += "The folder has size 0.\n";
            return result;
        }

        types.sort(); // сортировка типов по их названиям

        //вывод результатов
        for (int i = 0; i < types.size(); i++) {
            result += types[i] + ", size percentage: " + QString::number(((double)hash[types[i]] / totalSize) * 100) + "%\n";
        }
    } else { // обработка файла, не являющегося папокй
        result += FileType(pathInfo) + ", size percentage: 100%\n";
    }
    return result;
}

// функция обработки вложенной папки
void FileTypeStrategy::FolderSize(const QString &path, QHash<QString, quint64> &hash) {
    QDir dir(path); // текущая директория
    quint64 temp = QFileInfo(path + "/.").size();
    if (temp) {
        hash[FileType(QFileInfo(path + "/."))] += temp;
    }
    //цикл по всем папкам в текущей папке
    foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
    {
        if (folder.isSymLink()) { // проверка на ссылку
            if (FileType(folder) == ".lnk") {
                hash[FileType(folder)] += folder.size();
            }
        } else {
            FolderSize(folder.path() + '/' + folder.fileName(), hash); // проводятся вычисления с вложенной папкой
        }
    }

    //цикл по всем файлам в папке
    foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
    {
        hash[FileType(file)] += file.size(); // вычисляется размер файла
    }
}

// функция определения типа файла
QString FileTypeStrategy::FileType(const QFileInfo &file) {
    if (file.isSymLink()) { // ссылка
        return "symlink";
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

    return fileName.mid(i); // возвращение типа файла
}
