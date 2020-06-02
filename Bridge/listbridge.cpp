#include "listbridge.h"

ListBridge::ListBridge(QObject *p): AbstractBridge(p) {
    view = new QTreeView();
    model = new FileSizeDataModel();
};

ListBridge::~ListBridge() {
    delete view;
    delete model;
}

QWidget* ListBridge::UpdateData(QList<FileSizeData> data) {
    delete model;
    model = new FileSizeDataModel(parent, data);
    view->setModel(model);
    return view;
}
