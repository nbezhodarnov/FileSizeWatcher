#include "barbridge.h"

BarBridge::BarBridge(QObject *p): AbstractBridge(p) {
    view = new QtCharts::QChartView();
    model = new QtCharts::QChart();
};

BarBridge::~BarBridge(){
    delete view;
    //delete model;
}

QWidget* BarBridge::UpdateData(QList<FileSizeData> data){
    QtCharts::QChart *newModel = new QtCharts::QChart();
    FileSizeData others("Другие", 0, 0.0);
    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    for (auto iterator = data.begin(); iterator != data.end(); iterator++) {
        if (iterator->sizePercentage < 1) {
            others.size += iterator->size;
            others.sizePercentage += iterator->sizePercentage;
            continue;
        }
        QtCharts::QBarSet *set = new QtCharts::QBarSet(iterator->FileInfo + " (" + QString::number(iterator->sizePercentage, 'f', 2) + "%)");
        set->operator<<(iterator->sizePercentage);
        series->append(set);
    }
    if (others.size > 0) {
        QtCharts::QBarSet *set = new QtCharts::QBarSet(others.FileInfo + " (" + QString::number(others.sizePercentage, 'f', 2) + "%)");
        set->operator<<(others.sizePercentage);
        series->append(set);
    }
    newModel->addSeries(series);
    newModel->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setRange(0,100);
    newModel->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    newModel->legend()->setVisible(true);
    newModel->legend()->setAlignment(Qt::AlignRight);
    QtCharts::QChart *modelTemp = model;
    model = newModel;
    view->setChart(this->model);
    delete modelTemp;
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}
