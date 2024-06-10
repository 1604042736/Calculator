#include <QApplication>
#include <iostream>

#include "Generator.h"
#include "ExprSymbol.h"
#include "Figure.h"
#include "Integer.h"
#include "Sin.h"
#include "Relation.h"
#include "Equality.h"

class PlotMapping : public Mapping
{
public:
    PlotMapping() : Mapping("plot") {}

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1 || !isinstance<Relation>(args[0]))
            throw std::runtime_error("[PlotMapping]超出定义域");
        Relation *r = dynamic_cast<Relation *>(args[0].get());
        exprptr_t expr1 = r->lhs->simplify();
        exprptr_t expr2 = r->rhs->simplify();

        Generator::Relation relation;
        if (isinstance<Equality>(args[0]))
            relation = Generator::Eq;
        else
            throw std::runtime_error("不支持的关系");

        int argc = 1;
        char *argv[] = {(char *)"Plot"};
        QApplication app(argc, argv);

        auto data = Generator().generate(expr1,
                                         expr2,
                                         relation,
                                         QPointF(-10, 10),
                                         QPointF(-10, 10),
                                         QSize(1000, 1000));
        Figure w(data, QPoint(0, 0), QSize(1000, 1000));
        w.show();
        app.exec();
        return nullptr;
    }
};

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
    namespace Win
    {
#include "windows.h"
        void WINAPI load(Runtime *runtime)
        {
            runtime->defName("plot", objptr_t(new PlotMapping()));
        }
    };
    using Win::load;
#endif

#ifdef __cplusplus
}
#endif

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