#pragma once

#include <QObject>
#include <QPointF>
#include <QSize>
#include <QVector>
#include <QMap>

#include "Expression.h"

typedef QVector<QPair<QRect, QPointF>> data_t;

class Generator : public QObject
{
    Q_OBJECT
public:
    enum Relation
    {
        Eq
    };
    data_t generate(exprptr_t, exprptr_t, Relation, QPointF, QPointF, QSize);
};

setptr_t to_interval(QPointF);