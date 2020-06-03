#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QItemSelectionModel>
#include <QTableView>
#include <QTreeView>

// конструктор
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this); // настройка интерфейса окна
    ui->folderTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents); // установка задания размера первого столбца в зависимости от длины отображающихся элементов
    groupingStrategy = new FolderStrategy(); // задание стратегии группировки
    path = QDir::homePath(); // задание пути к домашней папке
    // создание модели файловой системы и формирование отображения дерева папок
    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden | QDir::System);
    dirModel->setRootPath(path);
    ui->folderTreeView->setModel(dirModel);
    ui->folderTreeView->expandAll();
    // скрытие ненужных столбцов
    ui->folderTreeView->hideColumn(1);
    ui->folderTreeView->hideColumn(3);
    // задание отображения
    bridge = new TableBridge(this);
    ui->splitter->addWidget(bridge->UpdateData(data));
    // соединение сигнала выбора директории со слотом отображения информации
    connect(ui->folderTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    //Пример организации установки курсора в TreeView относительно модельного индекса
    /*
    QModelIndex indexHomePath = dirModel->index(path);
    QFileInfo fileInfo = dirModel->fileInfo(indexHomePath);
    QItemSelection toggleSelection;
    QModelIndex topLeft;
    topLeft = dirModel->index(homePath);
    dirModel->setRootPath(homePath);

    toggleSelection.select(topLeft, topLeft);
    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
    */
}

// функция, организующая отображение данных в окне, на входе bool - флаг необходимости обновления данных, AbstractBridge* - указатель на отображение, на которое нужно сменить текущее
void MainWindow::infoShow(bool refreshData = true, AbstractBridge *br = nullptr) {
    if (refreshData) { // проверка на необходимость обновить данные
        data = groupingStrategy->Explore(path); // получение новых данных
        bridge->UpdateData(data); // обновление данных
    } else {
        QList<int> width = ui->splitter->sizes(); // сохранение размеров виджетов
        delete bridge; // удаление старого отображения
        bridge = br; // запоминание нужного отображения
        ui->splitter->addWidget(bridge->UpdateData(data)); // добавление виджета
        ui->splitter->setSizes(width); // задание размеров виджетов
    }
    this->statusBar()->showMessage("Выбранный путь: " + path); // отображение выбранного пути
}

// слот, активирующийся при выборе директории
void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);
    // получение индекса выбранной папки
    QModelIndexList indexs = selected.indexes();
    // получение пути выбранной папки
    QString filePath = "";
    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = dirModel->filePath(ix);
    }
    path = filePath; // сохранение пути
    // отображение данных
    infoShow();
}

// деструктор
MainWindow::~MainWindow() {
    delete ui;
    delete dirModel;
    delete groupingStrategy;
    delete bridge;
}

// слот, активирующийся при выборе группировки по элементам папки
void MainWindow::on_folder_triggered() {
    delete groupingStrategy;
    groupingStrategy = new FolderStrategy();
    infoShow();
}

// слот, активирующийся при выборе группировки по типам
void MainWindow::on_fileType_triggered() {
    delete groupingStrategy;
    groupingStrategy = new FileTypeStrategy();
    infoShow();
}

// слот, активирующийся при выборе отображения списком
void MainWindow::on_list_triggered() {
    infoShow(false, new ListBridge(this));
}

// слот, активирующийся при выборе отображения таблицей
void MainWindow::on_table_triggered() {
    infoShow(false, new TableBridge(this));
}

// слот, активирующийся при выборе отображения столбчатой диаграммой
void MainWindow::on_barChart_triggered() {
    infoShow(false, new BarBridge(this));
}

// слот, активирующийся при выборе отображения круговой диаграммой
void MainWindow::on_pieChart_triggered() {
    infoShow(false, new PieBridge(this));
}
