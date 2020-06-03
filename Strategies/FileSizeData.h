#ifndef FILESIZEDATA_H
#define FILESIZEDATA_H

#include <QString>

// Структура для хранения данных об элементе папки
struct FileSizeData {
    QString FileInfo; // Название файла или тип
    quint64 size; // Размер файла в байтах
    double sizePercentage; // Процентное соотношение к общему размеру папки
    // конструктор
    FileSizeData(QString info_in = "", quint64 size_in = 0, double sizePercentage_in = 0.0): FileInfo(info_in), size(size_in), sizePercentage(sizePercentage_in) {};
};

#endif // FILESIZEDATA_H
