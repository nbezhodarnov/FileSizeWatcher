#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemView>
#include <QFileSystemModel>
#include <QTableView>
#include <QListView>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString homePath = QDir::homePath();
    QFileSystemModel *dirModel, *fileModel;
    // Определим  файловой системы:
    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setRootPath(homePath);
    ui->folderTreeView->setModel(dirModel);
    ui->folderTreeView->expandAll();

    QAbstractItemView *view = new QListView(this);
    ui->splitter->addWidget(view);
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath("/home/nblaaa/PR");
    view->setModel(fileModel);
    delete view;
    view = new QTableView();
    view->setModel(fileModel);
    ui->splitter->addWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}
