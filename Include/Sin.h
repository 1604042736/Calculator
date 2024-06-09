#pragma once

#include "TrigFunction.h"
#include "ExprSymbol.h"

class Sin : public TrigFunction
{
public:
    Sin(exprptr_t);

    virtual Object *copyThis() { return new Sin(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Sin(*this)); }

    virtual exprptr_t diff(exprptr_t);

    virtual exprptr_t _simplify();

    virtual setptr_t belongto();
};

typedef SArgFuncMapping<ExprMapping, Expression, Sin, ExprSymbol> SinMapping;