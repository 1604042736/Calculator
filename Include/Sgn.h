#pragma once

#include "ExprFunction.h"
#include "EnumSet.h"
#include "Integer.h"

class Sgn : public SArgExprFunction
{
public:
    Sgn(exprptr_t arg) : SArgExprFunction("sgn", arg)
    {
        domain = setptr_t(new RealSet());
        range = setptr_t(new EnumSet({
            exprptr_t(new Integer(1)),
            exprptr_t(new Integer(0)),
            exprptr_t(new Integer(-1)),
        }));
    }

    virtual Object *copyThis() { return new Sgn(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Sgn(*this)); }

    virtual exprptr_t _simplify();
};