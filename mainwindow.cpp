#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QItemSelectionModel>
#include <QTableView>
#include <QListView>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

    view = new QListView(this);
    ui->splitter->addWidget(view);
    fileModel = new FileSizeDataModel(this, QList<FileSizeData>());
    view->setModel(fileModel);
    delete view;
    view = new QTableView();
    view->setModel(fileModel);
    ui->splitter->addWidget(view);

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

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    //Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QModelIndex index = ui->folderTreeView->selectionModel()->currentIndex();
    QModelIndexList indexs = selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = dirModel->filePath(ix);
        this->statusBar()->showMessage("Выбранный путь: " + dirModel->filePath(indexs.constFirst()));
    }

    path = filePath;

    delete fileModel;
    fileModel = new FileSizeDataModel(this, groupingStrategy->Explore(filePath));

    //TODO: !!!!!
    /*
    Тут простейшая обработка ширины первого столбца относительно длины названия папки.
    Это для удобства, что бы при выборе папки имя полностью отображалась в  первом столбце.
    Требуется доработка(переработка).
    */
    int length = 200;
    int dx = 30;

    if (dirModel->fileName(index).length() * dx > length) {
        length = length + dirModel->fileName(index).length() * dx;
    }

    ui->folderTreeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());
    view->setModel(fileModel);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dirModel;
    delete groupingStrategy;
    delete fileModel;
    delete view;
}

void MainWindow::on_folder_triggered()
{
    delete groupingStrategy;
    groupingStrategy = new FolderStrategy();
    delete fileModel;
    QModelIndex index = ui->folderTreeView->selectionModel()->currentIndex();
    fileModel = new FileSizeDataModel(this, groupingStrategy->Explore(path));
    int length = 200;
    int dx = 30;

    if (dirModel->fileName(index).length() * dx > length) {
        length = length + dirModel->fileName(index).length() * dx;
    }

    this->statusBar()->showMessage("Выбранный путь: " + path);

    ui->folderTreeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());
    view->setModel(fileModel);
}

void MainWindow::on_fileType_triggered()
{
    delete groupingStrategy;
    groupingStrategy = new FileTypeStrategy();
    delete fileModel;
    QModelIndex index = ui->folderTreeView->selectionModel()->currentIndex();
    fileModel = new FileSizeDataModel(this, groupingStrategy->Explore(path));
    int length = 200;
    int dx = 30;

    if (dirModel->fileName(index).length() * dx > length) {
        length = length + dirModel->fileName(index).length() * dx;
    }

    this->statusBar()->showMessage("Выбранный путь: " + path);

    ui->folderTreeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());
    view->setModel(fileModel);
}
