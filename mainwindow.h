#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractItemView>
#include <QFileSystemModel>
#include <QItemSelection>
#include <QMainWindow>

#include "DataModel/filesizedatamodel.h"
#include "Strategies/filetypestrategy.h"
#include "Strategies/folderstrategy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAbstractItemView *view;
    ExplorerStrategy *groupingStrategy;
    QFileSystemModel *dirModel;
    FileSizeDataModel *fileModel;
};

#endif // MAINWINDOW_H
