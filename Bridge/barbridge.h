#ifndef BARBRIDGE_H
#define BARBRIDGE_H

#include <QtCharts/QBarSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>

#include "Bridge/abstractbridge.h"

class BarBridge : public AbstractBridge
{
public:
    BarBridge(QObject *p = nullptr);
    QWidget* UpdateData(QList<FileSizeData>);
    ~BarBridge();
private:
    QtCharts::QChartView *view;
    QtCharts::QChart *model;
};

#endif // BARBRIDGE_H
