#pragma once

#include "Object.h"
#include "Expression.h"
#include "Number.h"
#include "Mul.h"
#include "Pow.h"
#include "Integer.h"
#include "Interval.h"
#include "ExprSymbol.h"

class Integer;
/*圆周率*/
class PI : public Number, public ExprSymbol
{
public:
    PI() : ExprSymbol("pi") {}

    virtual double toDouble() { return M_PI; }

    virtual std::string toLateX() { return "\\pi"; }

    virtual exprptr_t eval(Integer);

    virtual Object *copyThis() { return new PI(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new PI(*this)); }

    virtual setptr_t belongto() { return setptr_t(new Interval(exprptr_t(new Integer(3)), exprptr_t(new Integer(4)), true, true)); }
};