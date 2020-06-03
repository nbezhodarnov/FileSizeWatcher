#ifndef BARBRIDGE_H
#define BARBRIDGE_H

#include <QtCharts/QBarSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>

#include "Bridge/abstractbridge.h"

// Конкретный класс, служащий для отображения информации в виде столбчатой диаграммы
class BarBridge : public AbstractBridge
{
public:
    // конструктор
    BarBridge(QObject *p = nullptr);
    // конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде стоблчатой диаграммы
    QWidget* UpdateData(const QList<FileSizeData>&);
    // деструктор
    ~BarBridge();
private:
    QtCharts::QChartView *view; // указатель на виджет, отображающий информацию в виде стоблчатой диаграммы
    QtCharts::QChart *model; // указатель на модель, из которой строится столбчатая диаграмма
};

#endif // BARBRIDGE_H
