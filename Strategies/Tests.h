#ifndef TESTS_H
#define TESTS_H

#include <QTextStream>

#include "Strategies/filetypestrategy.h"
#include "Strategies/folderstrategy.h"

class Tests {
public:
    explicit Tests () {
        QTextStream out(stdout);
        QString test_results_expected[5][2];
        test_results_expected[0][0] = QString::fromUtf8("");
        test_results_expected[1][0] = QString::fromUtf8("");
        test_results_expected[2][0] = QString::fromUtf8("");
        test_results_expected[3][0] = QString::fromUtf8("The folder is empty.\n");
        test_results_expected[4][0] = QString::fromUtf8("Test5.txt, size percentage: 100%\n");
        test_results_expected[0][1] = QString::fromUtf8("");
        test_results_expected[1][1] = QString::fromUtf8("");
        test_results_expected[2][1] = QString::fromUtf8("");
        test_results_expected[3][1] = QString::fromUtf8("The folder is empty.\n");
        test_results_expected[4][1] = QString::fromUtf8(".txt, size percentage: 100%\n");
        QString path = QString::fromUtf8("../FileSizeWatcher_Part1/Tests/Test0");
        for (int i = 1; i < 6; i++) {
            switch (i) {
                case 5: {
                    path.append(".txt");
                }
                default: {
                    path[35] = QChar('0' + i);
                }
            }
            out << path << "\n\n";
            strategy = new FolderStrategy();
            out << strategy->Explore(path) << '\n';
            delete strategy;
            strategy = new FileTypeStrategy();
            out << strategy->Explore(path) << '\n';
            delete strategy;
            out << '\n' << flush;
        }
    }
private:
    ExplorerStrategy *strategy;
};

#endif // TESTS_H
