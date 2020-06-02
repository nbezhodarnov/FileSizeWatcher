#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QItemSelectionModel>
#include <QTableView>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    groupingStrategy = new FolderStrategy();
    QString homePath = QDir::homePath();
    path = homePath;
    // Определим  файловой системы:
    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden | QDir::System);
    dirModel->setRootPath(homePath);
    ui->folderTreeView->setModel(dirModel);
    ui->folderTreeView->expandAll();
    ui->folderTreeView->hideColumn(1);
    ui->folderTreeView->hideColumn(3);

    bridge = new TableBridge(this);
    ui->splitter->addWidget(bridge->UpdateData(data));

    QItemSelectionModel *selectionModel = ui->folderTreeView->selectionModel();

    QModelIndex indexHomePath = dirModel->index(homePath);
    QFileInfo fileInfo = dirModel->fileInfo(indexHomePath);

    ui->folderTreeView->header()->resizeSection(0, 200);
    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TreeView
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    //Пример организации установки курсора в TreeView относительно модельного индекса
    /*
    QItemSelection toggleSelection;
    QModelIndex topLeft;
    topLeft = dirModel->index(homePath);
    dirModel->setRootPath(homePath);

    toggleSelection.select(topLeft, topLeft);
    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
    */
}

void MainWindow::infoShow(bool refreshData = true, AbstractBridge *br = nullptr) {
    if (refreshData) {
        data = groupingStrategy->Explore(path);
        bridge->UpdateData(data);
    } else {
        QList<int> width = ui->splitter->sizes();
        delete bridge;
        bridge = br;
        ui->splitter->addWidget(bridge->UpdateData(data));
        ui->splitter->setSizes(width);
    }
    this->statusBar()->showMessage("Выбранный путь: " + path);
}

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected) {
    //Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QModelIndex index = ui->folderTreeView->selectionModel()->currentIndex();
    QModelIndexList indexs = selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = dirModel->filePath(ix);
        //this->statusBar()->showMessage("Выбранный путь: " + dirModel->filePath(indexs.constFirst()));
    }

    int length = 200;
    int dx = 30;
    //TODO: !!!!!
    /*
    Тут простейшая обработка ширины первого столбца относительно длины названия папки.
    Это для удобства, что бы при выборе папки имя полностью отображалась в  первом столбце.
    Требуется доработка(переработка).
    */
    if (dirModel->fileName(index).length() * dx > length) {
        length = length + dirModel->fileName(index).length() * dx;
    }
    ui->folderTreeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());

    path = filePath;

    infoShow();
}

MainWindow::~MainWindow() {
    delete ui;
    delete dirModel;
    delete groupingStrategy;
    delete bridge;
}

void MainWindow::on_folder_triggered() {
    delete groupingStrategy;
    groupingStrategy = new FolderStrategy();
    infoShow();
}

void MainWindow::on_fileType_triggered() {
    delete groupingStrategy;
    groupingStrategy = new FileTypeStrategy();
    infoShow();
}

void MainWindow::on_list_triggered() {
    infoShow(false, new ListBridge(this));
}

void MainWindow::on_table_triggered() {
    infoShow(false, new TableBridge(this));
}

void MainWindow::on_barChart_triggered() {
    infoShow(false, new BarBridge(this));
}

void MainWindow::on_pieChart_triggered() {
    infoShow(false, new PieBridge(this));
}
