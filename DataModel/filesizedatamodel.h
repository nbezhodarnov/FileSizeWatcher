#ifndef FILESIZEDATAMODEL_H
#define FILESIZEDATAMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QString>

#include "Strategies/FileSizeData.h"

// Конкретный класс, служащий моделью для отображения информации через MVC
// Является наследником QAbstractItemModel, что делает модель более универсальной
class FileSizeDataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    // конструктор
    FileSizeDataModel(QObject *parent = nullptr, const QList<FileSizeData> &dt = QList<FileSizeData>());
    // функция, которая возвращает количество строчек, на входе QModelIndex& - ссылка на индекс родителя, на выходе int - количество строчек
    int rowCount(const QModelIndex &parent) const override;
    // функция, которая возвращает количество столбцов, на входе QModelIndex& - ссылка на индекс родителя, на выходе int - количество столбцов
    int columnCount(const QModelIndex &parent) const override;
    // функция, которая возвращает элемент модели, на входе QModelIndex& - ссылка на индекс элемента и int - роль, на выходе QVariant - данные элемента модели
    QVariant data(const QModelIndex &index, int role) const override;
    // функция, которая возвращает заголовок элементов модели, на входе int - секция + Qt::Orientation - ориентация + int - роль, на выходе QVariant - данные заголовка элементов модели
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    // функция, которая возвращает индекс элемента (требуется для того, чтобы класс не был абстрактным), на входе int - строка + int - столбец + QModelIndex& - ссылка на индекс родителя, на выходе QModelIndex - индекс элемента
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    // функция, которая возвращает индекс родителя (требуется для того, чтобы класс не был абстрактным), на входе QModelIndex& - ссылка на индекс элемента, на выходе QModelIndex - индекс родителя
    QModelIndex parent(const QModelIndex &child) const override;
    // функция, которая позволяет изменять данные при необходимости (здесь определяется ради того, чтобы не допустить изменения данных), на входе QModelIndex& - ссылка на индекс элемента + QVariant& - ссылка на изменённые данные + int - роль, на выходе bool - флаг об успешности изменения данных
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    // функция, которая возвращает флаги элемента, QModelIndex& - ссылка на индекс элемента, на выходе Qt::ItemFlags - флаги
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // функция, возвращающая флаг о наличии потомков (требуется для универсальности модели), на входе QModelIndex& - ссылка на индекс элемента, на выходе bool - флаг о наличии потомков
    bool hasChildren(const QModelIndex &parent) const override;
private:
    // индексация столбцов модели
    enum NameColumn {
        NAME,
        SIZE,
        PERCENT
    };
    // данные модели
    QVector<FileSizeData> dataModel;
};

#endif
