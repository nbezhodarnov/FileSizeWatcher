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
#include "Bridge/piebridge.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    // слот, активирующийся при выборе директории
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    // слот, активирующийся при выборе группировки по элементам папки
    void on_folder_triggered();
    // слот, активирующийся при выборе группировки по типам
    void on_fileType_triggered();
    // слот, активирующийся при выборе отображения списком
    void on_list_triggered();
    // слот, активирующийся при выборе отображения таблицей
    void on_table_triggered();
    // слот, активирующийся при выборе отображения стобчатой диаграммой
    void on_barChart_triggered();
    // слот, активирующийся при выборе отображения круговой диаграммой
    void on_pieChart_triggered();

public:
    // конструктор
    explicit MainWindow(QWidget *parent = nullptr);
    // деструктор
    ~MainWindow();

private:
    Ui::MainWindow *ui; // интерфейс окна
    QFileSystemModel *dirModel; // модель файловой системы
    ExplorerStrategy *groupingStrategy; // указатель на стратегию
    QList<FileSizeData> data; // данные о размерах элементов папки
    AbstractBridge *bridge; // указатель на способ отображения
    QString path; // путь к нужной папке
    // функция, организующая отображение данных в окне, на входе bool - флаг необходимости обновления данных, AbstractBridge* - указатель на отображение, на которое нужно сменить текущее
    void infoShow(bool, AbstractBridge*);
};

#endif // MAINWINDOW_H
