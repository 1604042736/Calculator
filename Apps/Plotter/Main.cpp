#include <QApplication>
#include <iostream>

#include "Generator.h"
#include "ExprSymbol.h"
#include "Figure.h"
#include "Integer.h"
#include "Sin.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    exprptr_t x(new ExprSymbol("x"));
    exprptr_t y(new ExprSymbol("y"));
    auto data = Generator().generate(exprptr_t(new Sin(x)),
                                     y,
                                     Generator::Eq,
                                     QPointF(-10, 10),
                                     QPointF(-10, 10),
                                     QSize(1000, 1000));
    Figure w(data, QPoint(0, 0), QSize(1000, 1000));
    w.show();
    return app.exec();
}