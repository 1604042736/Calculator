#pragma once

#include "TrigFunction.h"
#include "ExprMapping.h"

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

class TanMapping : public ExprMapping
{
public:
    TanMapping()
        : ExprMapping("tan",
                      {},
                      setptr_t(new RealSet()),
                      setptr_t(new RealSet())) {}

    objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1 || !isinstance<Expression>(args[0]))
            throw std::runtime_error("[Tan]超出定义域");
        return Tan(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr()).simplify();
    }
};