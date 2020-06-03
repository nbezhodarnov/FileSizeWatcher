#include "piebridge.h"

// конструктор
PieBridge::PieBridge(QObject *p): AbstractBridge(p) {
    view = new QtCharts::QChartView(); // создание виджета
    model = new QtCharts::QChart(); // создание пустой модели (чтобы избежать ошибку SIGSEGV)
    // задание настроек рендера
    view->setRenderHint(QPainter::Antialiasing);
};

// деструктор
PieBridge::~PieBridge() {
    delete view; // удаление виджета
    // модель удалять не нужно, так как она была удалена вместе с виджетом (а удаление модели после виджета приведёт к ошибке SIGSEGV)
    //delete model;
}

// конкретная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в виде круговой диаграммы
QWidget* PieBridge::UpdateData(const QList<FileSizeData> &data) {
    QtCharts::QChart *newModel = new QtCharts::QChart(); // создание диаграммы
    FileSizeData others("Другие", 0, 0.0); // создание элемента "Другие"
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries(); // серия элементов диаграммы
    // цикл по всем элементам списка данных
    for (auto iterator = data.begin(); iterator != data.end(); iterator++) {
        if (iterator->sizePercentage < 1) { // проверка на малость элемента
            // заполнения элемента "Другие"
            others.size += iterator->size;
            others.sizePercentage += iterator->sizePercentage;
            continue;
        }
        // создание элемента диаграммы, занесение данных в серию
        series->append(new QtCharts::QPieSlice(iterator->FileInfo + " (" + QString::number(iterator->sizePercentage, 'f', 2) + "%)", iterator->sizePercentage));
    }
    if (others.size > 0) {
        // создание элемента диаграммы, занесение данных в серию
        series->append(new QtCharts::QPieSlice(others.FileInfo + " (" + QString::number(others.sizePercentage, 'f', 2) + "%)", others.sizePercentage));
    }
    newModel->addSeries(series); // занесение данных в диаграмму
    newModel->setAnimationOptions(QtCharts::QChart::SeriesAnimations); // задание настроек анимации
    // задание настроек легенды
    newModel->legend()->setVisible(true); // видимость легенды
    newModel->legend()->setAlignment(Qt::AlignRight); // расположение легенды

    // за памятью, выделенные под ранее созданные элементы, следит родитель

    // удаление предыдущей модели (так как виджет меняет модель, то наблюдаемость за утечкой памяти прошлой теряется, поэтому требуется самостоятельно удалять предыдущую модель)
    QtCharts::QChart *modelTemp = model;
    model = newModel;
    view->setChart(this->model);
    delete modelTemp;
    // возврат указателя на виджет
    return view;
}
