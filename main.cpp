#include <QCoreApplication>
#include <QTextStream>
#include <QFileInfo>
#include <QString>
#include <QList>
#include <QDir>

#include <iostream>

#include <QDebug>

class ExplorerStrategy
{
public:
    ExplorerStrategy() {};
    virtual void Explore (const QString &)=0;
};

class FolderStrategy : public ExplorerStrategy
{
public:
    FolderStrategy() {};
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
            foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name))
            {
                tempSize = FolderSize(folder.path() + '/' + folder.fileName()); // вычисляется размер папки
                sizes.append(tempSize);
                totalSize += tempSize;
            }
            //цикл по всем файлам в папке
            foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::NoSymLinks, QDir::Name))
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
            foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::NoSymLinks, QDir::Name))
            {
                out << file.fileName() << ", size percentage: " << ((double)*iterator / totalSize) * 100 << "%\n" << flush;
                iterator++;
            }
        } else {
            out << pathInfo.fileName() << ", size percentage: 100%\n" << flush;
        }
        out << '\n' << flush;
    }

private:
    quint64 FolderSize(const QString &path) {
        QDir dir(path); // текущая директория
        quint64 size = QFileInfo(path + "/.").size(); // объявление переменной, отвечающей за размер текущей папки (начальное значение задаётся такое, чтобы вычислить реальный размер папки)

        //цикл по всем папкам в текущей папке
        foreach (QFileInfo folder, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden))
        {
            size += FolderSize(folder.path() + '/' + folder.fileName()); // вычисляется размер папки
        }

        //цикл по всем файлам в папке
        foreach (QFileInfo file, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::NoSymLinks))
        {
            size += file.size(); // вычисляется размер файла
        }

        return size; // получаем итоговый размер
    }
};

/*class MainWindow : public ??????
{
    ExplorerStrategy explorer;
    ExplorerStrategy folderStrategy = new FolderStrategy ();
    ExplorerStrategy fileTypeStrategy = new FileTypeStrategy
            ();
    ExplorerStrategy newStrategy = new NewAlgorithm ();
    switch (...)
{
    case 0: explorer = folderStrategy; break;
    case 1: explorer = fileTypeStrategy; break;
    case 2: explorer = newStrategy; break;
}
explorer.Explore (...);
}*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDir myDir;
    QString info;
    //myDir = QDir::home();
    // myDir = QDir::current();
    //myDir.setPath("E"+QString(QDir::separator())+"TestA");
    myDir.setPath(QString::fromUtf8("/home/nblaaa/Рабочий стол/Лабы/6 семестр/Системное и прикладное ПО/Лабораторная 3/FileSizeWatcher_Part1/"));
    info = myDir.path();
    ExplorerStrategy *strategy = new FolderStrategy();
    strategy->Explore(info);

    QFileInfo fileInfo(myDir.path());

    return 0;//a.exec();
}
