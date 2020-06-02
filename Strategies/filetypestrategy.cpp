#include "filetypestrategy.h"

#include <QTextStream>
#include <QFileInfo>
#include <QString>
#include <QFile>
#include <QDir>

// конкретная функция вычисления информации о размерах содержимого папки (с группировкой по типам данных)
// на вход подаётся QString - путь к папке, на выходе QList<FileSizeData> - список данных о размере, занимаемом каждым типом в папке
QList<FileSizeData> FileTypeStrategy::Explore(const QString &path) {
    QFileInfo pathInfo(path);
    QTextStream out(stdout);
    QList<FileSizeData> result; // список данных о размере, занимаемом каждым типом в папке

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
        QHash<QString, quint64> hash; // информация о размере в байтах, занимаемом каждым типом (QString - тип, quint64 - размер в байтах)
        #if not defined(Q_OS_WIN)
        quint64 temp = QFileInfo(pathInfo.absoluteFilePath() + '.').size();
        if (temp) {
            hash[FileType(QFileInfo(pathInfo.absoluteFilePath() + '.'))] = temp;
        }
        #endif

        //вычисление размеров объектов
        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System | QDir::NoSymLinks))
        {
            FolderSize(folder.path() + '/' + folder.fileName(), hash); // проводятся вычисления с папкой
        }
        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
        {
            if (file.isSymLink()) { // проверка на ярлык
                QFile fileOpen(file.absoluteFilePath());
                fileOpen.open(QIODevice::ReadOnly); // в силу работы компилятора для вычисления размера ярлыка необходимо открыть его
                hash[FileType(file)] += fileOpen.size(); // вычисляется размер файла
                fileOpen.close();
                continue;
            }
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
            out << "The folder has size 0.\n" << flush;
            return QList<FileSizeData>();
        }

        types.sort(); // сортировка типов по их названиям

        //сохранение результатов
        for (int i = 0; i < types.size(); i++) {
            result.append(FileSizeData(types[i], hash[types[i]], ((double)hash[types[i]] / totalSize) * 100));
        }
    } else { // обработка файла, не являющегося папокй
        quint64 fileSize = pathInfo.size(); // вычисляется размер файла
        #if defined(Q_OS_WIN)
        if (pathInfo.isSymLink()) { // проверка на ярлык
            QFile fileOpen(pathInfo.absoluteFilePath());
            fileOpen.open(QIODevice::ReadOnly); // в силу работы компилятора для вычисления размера ярлыка необходимо открыть его
            fileSize = fileOpen.size(); // вычисляется размер файла
            fileOpen.close();
        }
        #endif
        result.append(FileSizeData(FileType(pathInfo), fileSize, 100));
    }
    return result;
}

// функция вычисления размера вложенной папки
// на вход подаётся QString - путь к папке, ссылка на QHash<QString, quint64> - объект, содержащий информацию о размере, занимаемом каждым типом в папке (QString - тип данных, quint64 - размер в байтах)
void FileTypeStrategy::FolderSize(const QString &path, QHash<QString, quint64> &hash) {
    QDir dir(path); // текущая директория
    #if not defined(Q_OS_WIN)
    quint64 temp = QFileInfo(path + "/.").size();
    if (temp) {
        hash[FileType(QFileInfo(path + "/."))] += temp;
    }
    #endif
    //цикл по всем папкам в текущей папке
    foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System | QDir::NoSymLinks))
    {
        FolderSize(folder.path() + '/' + folder.fileName(), hash); // проводятся вычисления с вложенной папкой
    }

    //цикл по всем файлам в папке
    foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
    {
        if (file.isSymLink()) { // проверка на ярлык
            QFile fileOpen(file.absoluteFilePath());
            fileOpen.open(QIODevice::ReadOnly); // в силу работы компилятора для вычисления размера ярлыка необходимо открыть его
            hash[FileType(file)] += fileOpen.size(); // вычисляется размер файла
            fileOpen.close();
            continue;
        }
        hash[FileType(file)] += file.size(); // вычисляется размер файла
    }
}

// функция определения типа файла
// на вход подаётся QFileInfo - информация о файле, на выходе QString - тип файла
QString FileTypeStrategy::FileType(const QFileInfo &file) {
    if (file.isSymLink()) { // ссылка
        if (file.fileName().mid(file.fileName().lastIndexOf('.') + 1) == "lnk") { // ярлык
            return ".lnk";
        }
        return "symlink";
    }
    if (file.isDir()) { // папка
        return "directory";
    }

    QString fileName = file.fileName(); // название файла
    int i = fileName.lastIndexOf('.'); // поиск символа .

    if (i == -1) { // неизвестный тип (отсутствует символ .)
        return "unknown";
    }

    return fileName.mid(i); // возвращение типа файла
}
