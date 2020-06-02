#include "tablebridge.h"

TableBridge::TableBridge(QObject *p): AbstractBridge(p) {
    view = new QTableView();
    model = new FileSizeDataModel();
};

TableBridge::~TableBridge(){
    delete view;
    delete model;
}

QWidget* TableBridge::UpdateData(QList<FileSizeData> data){
    delete model;
    model = new FileSizeDataModel(parent, data);
    view->setModel(model);
    return view;
}
