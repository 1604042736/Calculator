#pragma once

#include "TrigFunction.h"

class Cos : public TrigFunction
{
public:
    Cos(exprptr_t);

    virtual Object *copyThis() { return new Cos(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Cos(*this)); }

    virtual exprptr_t diff(exprptr_t);

    virtual exprptr_t _simplify();

    virtual setptr_t belongto();
};

typedef SArgFuncMapping<ExprMapping, Expression, Cos, ExprSymbol> CosMapping;