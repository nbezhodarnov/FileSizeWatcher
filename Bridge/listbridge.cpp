#include "listbridge.h"

// конструктор
ListBridge::ListBridge(QObject *p): AbstractBridge(p) {
    view = new QTreeView(); // создание виджета
    model = new FileSizeDataModel(); // создание пустой модели (чтобы избежать ошибку SIGSEGV)
};

// деструктор
ListBridge::~ListBridge() {
    delete view; // удаление виджета
    delete model; // удаление модели
}

// конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде списка
QWidget* ListBridge::UpdateData(const QList<FileSizeData> &data) {
    delete model; // удаление предыдущей модели
    model = new FileSizeDataModel(parent, data); // создание новой модели
    view->setModel(model); // занесение модели в виджет
    // возврат указателя на виджет
    return view;
}
