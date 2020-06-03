#ifndef ABSTRACTBRIDGE_H
#define ABSTRACTBRIDGE_H

#include <QWidget>
#include <QList>

#include "Strategies/FileSizeData.h"

// Абстрактный класс, являющийся интерфейсом для отображения информации в нужном виде
class AbstractBridge
{
public:
    //конструктор
    AbstractBridge(QObject *p = nullptr): parent(p) {};
    // абстрактная функция обработки данных, на вход принимает QList<FileSizeData> - данные о размерах элементов папки, на выходе QWidget* - указатель на виджет, отображающий информацию в нужном виде
    virtual QWidget* UpdateData(const QList<FileSizeData>&) = 0;
    // виртуальный деструктор, позволяющий наследникам определять свои деструкторы
    virtual ~AbstractBridge() {};
protected:
    QObject *parent; // указатель на родителя
};

#endif // ABSTRACTBRIDGE_H
