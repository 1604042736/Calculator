#pragma once

#include "TrigFunction.h"

class Tan : public TrigFunction
{
public:
    Tan(exprptr_t);

    virtual Object *copyThis() { return new Tan(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Tan(*this)); }

    virtual exprptr_t diff(exprptr_t);

    virtual exprptr_t _simplify();

    virtual setptr_t belongto();
};

typedef SArgFuncMapping<ExprMapping, Expression, Tan, ExprSymbol> TanMapping;