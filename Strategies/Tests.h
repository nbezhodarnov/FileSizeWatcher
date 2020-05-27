#ifndef TESTS_H
#define TESTS_H

#include <QTextStream>
#include <QTextCodec>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Strategies/filetypestrategy.h"
#include "Strategies/folderstrategy.h"

// класс Tests, осуществяющий проверку стратегий на примерах
class Tests {
public:
    explicit Tests () {
        QTextStream out(stdout);
        #if defined(Q_OS_WIN)
        SetConsoleOutputCP(1251);
        #endif
        /*QString test_results_expected[7][2];
        test_results_expected[0][0] = QString::fromUtf8("Heroes-II-of-Might-and-Magic-2-icon.png, size percentage: 99.9442%\nТекстовый файл (1).txt, size percentage: 0.0363711%\nТекстовый файл.txt, size percentage: 0.0193979%\n");
        test_results_expected[1][0] = QString::fromUtf8("Новая папка, size percentage: 0%\nНовая папка (1), size percentage: 20.3378%\nHeroes-II-of-Might-and-Magic-2-icon.png, size percentage: 79.6177%\nТекстовый файл (1).txt, size percentage: 0.028974%\nТекстовый файл.txt, size percentage: 0.0154528%\n");
        test_results_expected[2][0] = QString::fromUtf8(".vs, size percentage: 99.8674%\nTests, size percentage: 0%\nНовая папка, size percentage: 0%\nНовая папка (1), size percentage: 0.0288267%\nНовая папка (2), size percentage: 0.000171811%\nHeroes-II-of-Might-and-Magic-2-icon.png, size percentage: 0.103384%\nTests.lnk, size percentage: 0.000171811%\nТекстовый файл (1).txt, size percentage: 3.76229e-05%\nТекстовый файл.txt, size percentage: 2.00656e-05%\n");
        test_results_expected[3][0] = QString::fromUtf8("The folder is empty.\n");
        test_results_expected[4][0] = QString::fromUtf8("The folder has size 0.\n");
        test_results_expected[5][0] = QString::fromUtf8("Test6.txt, size percentage: 100%\n");
        test_results_expected[6][0] = QString::fromUtf8("The object doesn\'t exist.\n");
        test_results_expected[0][1] = QString::fromUtf8(".png, size percentage: 99.9442%\n.txt, size percentage: 0.0557691%\n");
        test_results_expected[1][1] = QString::fromUtf8(".png, size percentage: 79.6177%\n.sav, size percentage: 20.2741%\n.txt, size percentage: 0.10817%\n");
        test_results_expected[2][1] = QString::fromUtf8(".db, size percentage: 15.4977%\n.ipch, size percentage: 84.3254%\n.lnk, size percentage: 0.000343623%\n.png, size percentage: 0.103384%\n.sav, size percentage: 0.0287439%\n.suo, size percentage: 0.0443047%\n.txt, size percentage: 0.000140459%\n");
        test_results_expected[3][1] = QString::fromUtf8("The folder is empty.\n");
        test_results_expected[4][1] = QString::fromUtf8("The folder has size 0.\n");
        test_results_expected[5][1] = QString::fromUtf8(".txt, size percentage: 100%\n");
        test_results_expected[6][1] = QString::fromUtf8("The object doesn\'t exist.\n");
        #if defined(Q_OS_LINUX)
        test_results_expected[0][0] = QString::fromUtf8("Heroes-II-of-Might-and-Magic-2-icon.png, size percentage: 95.2159%\nТекстовый файл (1).txt, size percentage: 0.0346504%\nТекстовый файл.txt, size percentage: 0.0184802%\n");
        test_results_expected[1][0] = QString::fromUtf8("Новая папка, size percentage: 3.41547%\nНовая папка (1), size percentage: 24.3902%\nHeroes-II-of-Might-and-Magic-2-icon.png, size percentage: 68.7405%\nТекстовый файл (1).txt, size percentage: 0.0250156%\nТекстовый файл.txt, size percentage: 0.0133417%\n");
        test_results_expected[2][0] = QString::fromUtf8(".vs, size percentage: 99.8418%\nTests, size percentage: 0%\nНовая папка, size percentage: 0.00513362%\nНовая папка (1), size percentage: 0.0390762%\nНовая папка (2), size percentage: 0.00530532%\nHeroes-II-of-Might-and-Magic-2-icon.png, size percentage: 0.10332%\nTests.lnk, size percentage: 0.000171705%\nТекстовый файл (1).txt, size percentage: 3.75997e-05%\nТекстовый файл.txt, size percentage: 2.00532e-05%\n");
        test_results_expected[4][0] = QString::fromUtf8("Текстовый файл.txt, size percentage: 0%\n");
        test_results_expected[0][1] = QString::fromUtf8(".png, size percentage: 95.2159%\n.txt, size percentage: 0.0531307%\ndirectory, size percentage: 4.73094%\n");
        test_results_expected[1][1] = QString::fromUtf8(".png, size percentage: 68.7405%\n.sav, size percentage: 17.5043%\n.txt, size percentage: 0.0933917%\ndirectory, size percentage: 13.6619%\n");
        test_results_expected[2][1] = QString::fromUtf8(".db, size percentage: 15.4881%\n.ipch, size percentage: 84.2735%\n.lnk, size percentage: 0.000343411%\n.png, size percentage: 0.10332%\n.sav, size percentage: 0.0287262%\n.suo, size percentage: 0.0442775%\n.txt, size percentage: 0.000140372%\ndirectory, size percentage: 0.0616034%\n");
        test_results_expected[4][1] = QString::fromUtf8(".txt, size percentage: 0%\ndirectory, size percentage: 100%\n");
        #endif*/
        // относительный путь к тестам
        QString path = QString::fromUtf8("../FileSizeWatcher_Part1/Tests/Test0");
        // перебор всех тестов
        for (int i = 1; i < 8; i++) {
            switch (i) {
                case 6: {
                    path.append(".txt");
                }
                default: {
                    path[35] = QChar('0' + i);
                }
            }
            // вывод
            out << path << "\n\n" << flush;
            strategy = new FolderStrategy();
            out << DataToString(strategy->Explore(path)) << '\n' << flush;
            delete strategy;
            strategy = new FileTypeStrategy();
            out << '\n' << flush;
            out << DataToString(strategy->Explore(path)) << '\n' << flush;
            delete strategy;
            out << '\n' << flush;
        }
    }
private:
    ExplorerStrategy *strategy;

    // функция преобразования полученных данных в строку для вывода на консоль
    // на входе QList<FileSizeData> - данные о размерах содержимого папки, на выходе QString - строка для вывода на консоль
    QString DataToString(QList<FileSizeData> data) {
        QString result;
        for (auto iterator = data.begin(); iterator != data.end(); iterator++) {
            result += iterator->FileInfo + ", size percentage: ";
            if (iterator->sizePercentage < 0.01 && iterator->sizePercentage > 0) {
                result += "< 0.01%\n";
            } else {
                result += QString::number(iterator->sizePercentage) + "%\n";
            }
        }
        return result;
    }
};

#endif // TESTS_H
