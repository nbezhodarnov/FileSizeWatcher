#include "barbridge.h"

// конструктор
BarBridge::BarBridge(QObject *p): AbstractBridge(p) {
    view = new QtCharts::QChartView(); // создание виджета
    model = new QtCharts::QChart(); // создание пустой модели (чтобы избежать ошибку SIGSEGV)
};

// деструктор
BarBridge::~BarBridge() {
    delete view; // удаление виджета
    // модель удалять не нужно, так как она была удалена вместе с виджетом (а удаление модели после виджета приведёт к ошибке SIGSEGV)
    //delete model;
}

// конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде стоблчатой диаграммы
QWidget* BarBridge::UpdateData(const QList<FileSizeData> &data) {
    QtCharts::QChart *newModel = new QtCharts::QChart(); // создание диаграммы
    FileSizeData others("Другие", 0, 0.0); // создание элемента списка "Другие"
    QtCharts::QBarSeries *series = new QtCharts::QBarSeries(); // серия элементов диаграммы
    // цикл по всем элементам списка данных
    for (auto iterator = data.begin(); iterator != data.end(); iterator++) {
        if (iterator->sizePercentage < 1) { // проверка на малость элемента
            // заполнения элемента "Другие"
            others.size += iterator->size;
            others.sizePercentage += iterator->sizePercentage;
            continue;
        }
        // создание элемента диаграммы, занесение данных в этот элемент и серию
        QtCharts::QBarSet *set = new QtCharts::QBarSet(iterator->FileInfo + " (" + QString::number(iterator->sizePercentage, 'f', 2) + "%)");
        set->operator<<(iterator->sizePercentage);
        series->append(set); // занесение элемента диаграммы в серию
    }
    if (others.size > 0) { // проверка на пустоту элемента "Другие"
        // создание элемента диаграммы, занесение данных в этот элемент и серию
        QtCharts::QBarSet *set = new QtCharts::QBarSet(others.FileInfo + " (" + QString::number(others.sizePercentage, 'f', 2) + "%)");
        set->operator<<(others.sizePercentage);
        series->append(set); // занесение элемента диаграммы в серию
    }
    newModel->addSeries(series); // занесение данных в диаграмму
    newModel->setAnimationOptions(QtCharts::QChart::SeriesAnimations); // задание настроек анимации
    // создание оси y для удобного отображения данных
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setRange(0,100);
    newModel->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    // задание настроек легенды
    newModel->legend()->setVisible(true); // видимость легенды
    newModel->legend()->setAlignment(Qt::AlignRight); // расположение легенды

    // за памятью, выделенные под ранее созданные элементы, следит родитель

    // удаление предыдущей модели (так как виджет меняет модель, то наблюдаемость за утечкой памяти прошлой теряется, поэтому требуется самостоятельно удалять предыдущую модель)
    QtCharts::QChart *modelTemp = model;
    model = newModel;
    view->setChart(this->model);
    delete modelTemp;
    // задание настроек рендера
    view->setRenderHint(QPainter::Antialiasing);
    // возврат указателя на виджет
    return view;
}
