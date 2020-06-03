#include "tablebridge.h"

// конструктор
TableBridge::TableBridge(QObject *p): AbstractBridge(p) {
    QTableView *view_ptr = new QTableView(); // создание виджета
    model = new FileSizeDataModel(); // создание пустой модели (чтобы избежать ошибку SIGSEGV)
    // установление отображение на всю ширину и высоту виджета
    view_ptr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // сохранение виджета
    view = view_ptr;
};

// деструктор
TableBridge::~TableBridge() {
    delete view; // удаление виджета
    delete model; // удаление модели
}

// конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде таблицы
QWidget* TableBridge::UpdateData(const QList<FileSizeData> &data) {
    delete model; // удаление предыдущей модели
    model = new FileSizeDataModel(parent, data); // создание новой модели
    view->setModel(model); // занесение модели в виджет
    // возврат указателя на виджет
    return view;
}
