#pragma once

#include "ExprFunction.h"
#include "EnumSet.h"
#include "Integer.h"

class Sgn : public ExprFunction
{
public:
    Sgn(exprptr_t arg)
        : arg(arg),
          ExprFunction(setptr_t(new RealSet()),
                       setptr_t(new EnumSet({
                           exprptr_t(new Integer(1)),
                           exprptr_t(new Integer(0)),
                           exprptr_t(new Integer(-1)),
                       })))
    {
    }
    virtual std::string toString() { return "sgn(" + this->arg->toString() + ")"; }

    virtual Object *copyThis() { return new Sgn(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Sgn(*this)); }

    virtual boolptr_t operator==(exprptr_t);

    virtual objptr_t replace(objptr_t, objptr_t);

    virtual exprptr_t _simplify();

    exprptr_t arg;
};