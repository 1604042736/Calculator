#include <iostream>
#include <QDebug>
#include <QQueue>
#include <QRect>

#include "Generator.h"
#include "ExprSymbol.h"
#include "Interval.h"
#include "Float.h"
#include "Common.h"
#include "True.h"
#include "EmptySet.h"

setptr_t to_interval(QPointF a)
{
    exprptr_t x(new Float(QString::number(a.x()).toStdString()));
    exprptr_t y(new Float(QString::number(a.y()).toStdString()));
    return setptr_t(new Interval(x, y, true, true));
}

QPointF eq(Float a1, Float a2, Float b1, Float b2)
{
    if (a1 == b1 && a2 == b2)
        return QPointF(1, 1);
    else if ((a2 > b1 && b1 > a1) || (b2 > a1 && a1 > b1))
        return QPointF(0, 1);
    else
        return QPointF(0, 0);
}

QPointF eq(Interval *a, Interval *b)
{
    if (isinstance<Float>(a->start) && isinstance<Float>(a->end) && isinstance<Float>(b->start) && isinstance<Float>(b->end))
        return eq(*dynamic_cast<Float *>(a->start.get()),
                  *dynamic_cast<Float *>(a->end.get()),
                  *dynamic_cast<Float *>(b->start.get()),
                  *dynamic_cast<Float *>(b->end.get()));
    std::cout << "[eq]使用默认返回值\n";
    return QPointF(0, 1);
}

QPointF eq(Set *a, Set *b)
{
    if (isinstance<Interval>(a) && isinstance<Interval>(b))
        return eq(dynamic_cast<Interval *>(a), dynamic_cast<Interval *>(b));
    return QPointF(0, 1);
}

data_t
Generator::generate(exprptr_t expr1, exprptr_t expr2, Relation relation, QPointF x, QPointF y, QSize size)
{
    data_t data;
    QQueue<QPair<QPoint, QPoint>> que;
    QVector<QPair<QPoint, QPoint>> memory;
    que.enqueue(qMakePair(QPoint(0, size.width()), QPoint(0, size.height())));
    while (!que.empty())
    {
        auto h = que.dequeue();
        memory.push_back(h);
        // qDebug() << que.size();
        QPointF nx(x.x() + h.first.x() * (x.y() - x.x()) / size.width(),
                   x.x() + h.first.y() * (x.y() - x.x()) / size.width());
        QPointF ny(y.x() + h.second.x() * (y.y() - y.x()) / size.height(),
                   y.x() + h.second.y() * (y.y() - y.x()) / size.height());
        exprptr_t sx(new ExprSymbol("x", to_interval(nx)));
        exprptr_t sy(new ExprSymbol("y", to_interval(ny)));
        exprptr_t nexpr1 = dynamic_cast<Expression *>(expr1->replace(sx, sx)->replace(sy, sy).get())->copyToExprPtr();
        exprptr_t nexpr2 = dynamic_cast<Expression *>(expr2->replace(sx, sx)->replace(sy, sy).get())->copyToExprPtr();
        setptr_t s1 = nexpr1->belongto();
        setptr_t s2 = nexpr2->belongto();
        // qDebug() << QString::fromStdString(s1->toString()) << QString::fromStdString(s2->toString());
        QPointF p;
        switch (relation)
        {
        case Eq:
            if (isinstance<True>(s1->operator==(s2)))
                p = QPointF(1, 1);
            else if (isinstance<EmptySet>(s1->operator&(s2)))
                p = QPointF(0, 0);
            else
                p = QPointF(0, 1);
            break;
        }
        // qDebug() << nx << ny << p;
        /*data.push_back(qMakePair(QRect(h.first.x(),
                                       h.second.x(),
                                       h.first.y() - h.first.x(),
                                       h.second.y() - h.second.x()),
                                 p));*/
        if (p == QPointF(0, 0))
            continue;
        if (p == QPointF(1, 1))
        {
            // qDebug() << nx << ny << QString::fromStdString(s1->toString()) << QString::fromStdString(s2->toString());
        }
        int midx = (h.first.y() + h.first.x()) / 2;
        int midy = (h.second.y() + h.second.x()) / 2;

        if (h.first.y() - h.first.x() <= 0 || h.second.y() - h.second.x() <= 0)
        {
            // continue;
            data.push_back(qMakePair(QRect(h.first.x(),
                                           h.second.x(),
                                           h.first.y() - h.first.x(),
                                           h.second.y() - h.second.x()),
                                     p));
            continue;
        }

        for (auto t : {
                 qMakePair(QPoint(h.first.x(), midx), QPoint(h.second.x(), midy)),
                 qMakePair(QPoint(h.first.x(), midx), QPoint(midy, h.second.y())),
                 qMakePair(QPoint(midx, h.first.y()), QPoint(h.second.x(), midy)),
                 qMakePair(QPoint(midx, h.first.y()), QPoint(midy, h.second.y())),
             })
        {
            if (!memory.contains(t))
            {
                que.enqueue(t);
            }
        }
    }
    return data;
}