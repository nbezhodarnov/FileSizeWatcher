#include <QApplication>
#include <QTextStream>
#include <QString>

#include "Strategies/filetypestrategy.h"
#include "Strategies/folderstrategy.h"
#include "Strategies/Tests.h"

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //Tests tests; // запуск тестов

    MainWindow window; // создание окна
    window.show(); // отображение окна

    return a.exec();
}
