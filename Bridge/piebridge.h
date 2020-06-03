#ifndef PIEBRIDGE_H
#define PIEBRIDGE_H

#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>

#include "Bridge/abstractbridge.h"

// Конкретный класс, служащий для отображения информации в виде круговой диаграммы
class PieBridge : public AbstractBridge
{
public:
    // конструктор
    PieBridge(QObject *p = nullptr);
    // конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде круговой диаграммы
    QWidget* UpdateData(const QList<FileSizeData>&);
    // деструктор
    ~PieBridge();
private:
    QtCharts::QChartView *view;  // указатель на виджет, отображающий информацию в виде круговой диаграммы
    QtCharts::QChart *model; // указатель на модель, из которой строится круговая диаграмма
};

#endif // PIEBRIDGE_H
