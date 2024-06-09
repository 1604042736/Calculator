#pragma once

#include "Object.h"
#include "Expression.h"
#include "Number.h"
#include "Mul.h"
#include "Integer.h"
#include "Pow.h"
#include "Interval.h"
#include "ExprSymbol.h"

/*自然常数*/
class E : public Number, public ExprSymbol
{
public:
    E() : ExprSymbol("e") {}

    virtual double toDouble() { return 2.7182818284590452; }

    virtual exprptr_t eval(Integer);

    virtual Object *copyThis() { return new E(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new E(*this)); }

    virtual setptr_t belongto() { return setptr_t(new Interval(exprptr_t(new Integer(2)), exprptr_t(new Integer(3)), true, true)); }
};