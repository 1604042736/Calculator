#pragma once

#include <QWidget>
#include <QVector>
#include <QPaintEvent>
#include <QTimer>

#include "Generator.h"

class Figure : public QWidget
{
public:
    Figure(data_t, QPoint, QSize, QSize size = QSize(1000, 618));

    void paintEvent(QPaintEvent *);

    data_t data;
    QSize dsize; // data的size
    QPoint o;    // 原点坐标
    int data_index;
    QTimer timer;
};