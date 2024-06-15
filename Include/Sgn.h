#pragma once

#include "ExprFunction.h"

class Sgn : public ExprFunction
{
public:
    Sgn(exprptr_t arg) : arg(arg), ExprFunction("sgn", {arg}) {}

    virtual Object *copyThis() { return new Sgn(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Sgn(*this)); }

    virtual exprptr_t _simplify();

    virtual void setArgs(funcargs_t args)
    {
        this->arg = dynamic_cast<Expression *>(args[0].get())->copyToExprPtr();
        ExprFunction::setArgs(args);
    }

    exprptr_t arg;
};