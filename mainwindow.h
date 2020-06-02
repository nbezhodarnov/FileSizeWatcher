#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractItemView>
#include <QFileSystemModel>
#include <QItemSelection>
#include <QMainWindow>

#include "DataModel/filesizedatamodel.h"
#include "Strategies/filetypestrategy.h"
#include "Strategies/folderstrategy.h"
#include "Bridge/tablebridge.h"
#include "Bridge/listbridge.h"
#include "Bridge/barbridge.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);

    void on_folder_triggered();

    void on_fileType_triggered();

    void on_list_triggered();

    void on_table_triggered();

    void on_barChart_triggered();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QFileSystemModel *dirModel;

    ExplorerStrategy *groupingStrategy;
    QList<FileSizeData> data;
    AbstractBridge *bridge;
    QString path;

    void infoShow(bool, AbstractBridge*);
};

#endif // MAINWINDOW_H
