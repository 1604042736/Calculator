#include <iostream>
#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <cmath>

#include "Figure.h"

Figure::Figure(data_t data, QPoint o, QSize dsize, QSize size)
    : data(data), dsize(dsize), o(o), timer(this)
{
    resize(size);
    data_index = 0;
    connect(&timer, &QTimer::timeout, this, [&]()
            {if(data_index++>=data.size())timer.stop();
            update(); });
    timer.start(1);
}

void Figure::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    auto it = data.constBegin();
    double w = (double)width() / dsize.width();
    double h = (double)height() / dsize.height();
    // qDebug() << w << h;
    for (int i = 0; i < data_index && i < data.size(); i++)
    {
        auto point = data[i].second;
        if (point == QPointF(1, 1))
            painter.setBrush(QBrush(QColor(0, 0, 0)));
        else if (point == QPointF(0, 1))
            painter.setBrush(QBrush(QColor(255, 0, 0)));
        else if (point == QPointF(0, 0))
            painter.setBrush(QBrush(QColor(255, 255, 255)));
        painter.setPen(painter.brush().color());
        // qDebug() << data[i].first << point;
        painter.drawRect((o.x() + data[i].first.x()) * w,
                         (o.y() + ((double)dsize.height() - data[i].first.y())) * h,
                         std::max(((data[i].first.width()) * w), 1.0),
                         std::max(((data[i].first.height()) * h), 1.0));
    }
}