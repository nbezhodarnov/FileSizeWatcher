#include <QCoreApplication>
#include <QTextStream>
#include <QFileInfo>
#include <QString>
#include <QList>
#include <QHash>
#include <QDir>

// Абстрактный класс ExplorerStrategy, используемый для определения стратегий
class ExplorerStrategy
{
public:
    ExplorerStrategy() {};
    virtual void Explore (const QString &)=0;
    virtual ~ExplorerStrategy() {};
};

// Конкретный класс FolderStrategy, обрабатывающий папку с группировкой по папкам
class FolderStrategy : public ExplorerStrategy
{
public:
    FolderStrategy() {};

    // функция вывода информации о содержимом папки
    void Explore (const QString &path)
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

        if (pathInfo.isDir()) {
            // подготовка строки к работе
            QString pathAddition(".");
            if (path.at(path.size() - 1) != '/') {
                pathAddition.insert(0, '/');
            }

            QDir dir(path);
            quint64 totalSize = QFileInfo(path + pathAddition).size(), tempSize; // 1 - итоговый размер папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки), 2 - временная переменная
            QList<quint64> sizes; // массив размеров объектов

            //вычисление размеров объектов
            //цикл по всем папкам в текущей папке
            foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name | QDir::Type))
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
            foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name | QDir::Type))
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

private:
    // функция вычисления размера папки
    quint64 FolderSize(const QString &path) {
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
};

// Конкретный класс FileTypeStrategy, обрабатывающий папку с группировкой по типам
class FileTypeStrategy : public ExplorerStrategy
{
public:
    FileTypeStrategy() {};

    //функция вывода информации о содержимом папки
    void Explore (const QString &path)
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

        if (pathInfo.isDir()) {
            // подготовка строки к работе
            QString pathAddition(".");
            if (path.at(path.size() - 1) != '/') {
                pathAddition.insert(0, '/');
            }

            QDir dir(path);
            QHash<QString, quint64> hash;
            hash[FileType(path + pathAddition)] = QFileInfo(path + pathAddition).size();

            //вычисление размеров объектов
            //цикл по всем папкам в текущей папке
            foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name | QDir::Type))
            {
                if (folder.isSymLink()) { // проверка на ссылку
                    hash[FileType(folder)] = folder.size();
                } else {
                    FolderSize(folder.path() + '/' + folder.fileName(), hash); // проводятся вычисления с папкой
                }
            }
            //цикл по всем файлам в папке
            foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
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

private:
    // функция обработки вложенной папки
    void FolderSize(const QString &path, QHash<QString, quint64> &hash) {
        QDir dir(path); // текущая директория
        hash[FileType(QFileInfo(path + "/."))] += QFileInfo(path + "/.").size();
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
    QString FileType(const QFileInfo &file) {
        if (file.isDir()) { // папка
            return "directory";
        }
        if (file.isSymbolicLink()) { // ссылка
            return "symlink";
        }
        if (file.isShortcut()) { // ярлык
            return "shortcut";
        }

        QString fileName = file.fileName(); // название файла
        int i = fileName.size() - 1;
        for (; (i >= 0) && (fileName[i] != '.'); i--); // поиск символа .

        if (i < 0) { // неизвестный тип (отсутствует символ .)
            return "unknown";
        }

        QString type; // тип файла
        //заполнение типа файла
        for (; i < fileName.size(); i++) {
            type.append(fileName[i]);
        }
        return type;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDir myDir;
    QString info;
    myDir.setPath(QString::fromUtf8("/home/nblaaa/Рабочий стол/Лабы/6 семестр/Системное и прикладное ПО/Лабораторная 3/FileSizeWatcher_Part1/"));
    info = myDir.path();
    ExplorerStrategy *strategy = new FolderStrategy();
    strategy->Explore(info);
    delete strategy;
    strategy = new FileTypeStrategy();
    strategy->Explore(info);
    delete strategy;

    QFileInfo fileInfo(myDir.path());

    return 0;//a.exec();
}
