#include <QCoreApplication>
#include <QTextStream>
#include <QString>

#include "Strategies/filetypestrategy.h"
#include "Strategies/folderstrategy.h"
#include "Strategies/Tests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Tests tests; // запуск тестов
    return 0;//a.exec();
}
