#ifndef PIEBRIDGE_H
#define PIEBRIDGE_H

#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>

#include "Bridge/abstractbridge.h"

class PieBridge : public AbstractBridge
{
public:
    PieBridge(QObject *p = nullptr);
    QWidget* UpdateData(QList<FileSizeData>);
    ~PieBridge();
private:
    QtCharts::QChartView *view;
    QtCharts::QChart *model;
};

#endif // PIEBRIDGE_H
