#pragma once

#include "ExprFunction.h"

/*绝对值函数*/
class Abs : public ExprFunction
{
public:
    Abs(exprptr_t);

    virtual Object *copyThis() { return new Abs(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Abs(*this)); }

    virtual exprptr_t abs() { return this->copyToExprPtr(); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    exprptr_t arg;
};