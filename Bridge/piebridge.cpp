#include "piebridge.h"

PieBridge::PieBridge(QObject *p): AbstractBridge(p) {
    view = new QtCharts::QChartView();
    model = new QtCharts::QChart();
};

PieBridge::~PieBridge() {
    delete view;
    //delete model;
}

QWidget* PieBridge::UpdateData(QList<FileSizeData> data) {
    QtCharts::QChart *newModel = new QtCharts::QChart();
    FileSizeData others("Другие", 0, 0.0);
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    for (auto iterator = data.begin(); iterator != data.end(); iterator++) {
        if (iterator->sizePercentage < 1) {
            others.size += iterator->size;
            others.sizePercentage += iterator->sizePercentage;
            continue;
        }
        series->append(new QtCharts::QPieSlice(iterator->FileInfo + " (" + QString::number(iterator->sizePercentage, 'f', 2) + "%)", iterator->sizePercentage));
    }
    if (others.size > 0) {
        series->append(new QtCharts::QPieSlice(others.FileInfo + " (" + QString::number(others.sizePercentage, 'f', 2) + "%)", others.sizePercentage));
    }
    newModel->addSeries(series);
    newModel->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    newModel->legend()->setVisible(true);
    newModel->legend()->setAlignment(Qt::AlignRight);
    QtCharts::QChart *modelTemp = model;
    model = newModel;
    view->setChart(this->model);
    delete modelTemp;
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}
