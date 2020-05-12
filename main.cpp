#include <QCoreApplication>
#include <QTextStream>
#include <QString>

#include "Strategies/filetypestrategy.h"
#include "Strategies/folderstrategy.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString path = QString::fromUtf8("/home/nblaaa/Рабочий стол/Лабы/6 семестр/Системное и прикладное ПО/Лабораторная 3/FileSizeWatcher_Part1/");
    ExplorerStrategy *strategy = new FolderStrategy();
    strategy->Explore(path);
    delete strategy;
    strategy = new FileTypeStrategy();
    strategy->Explore(path);
    delete strategy;

    return 0;//a.exec();
}
