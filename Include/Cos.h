#pragma once

#include "TrigFunction.h"
#include "ExprMapping.h"

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

class CosMapping : public ExprMapping
{
public:
    CosMapping()
        : ExprMapping("cos",
                      {},
                      setptr_t(new RealSet()),
                      setptr_t(new Interval(exprptr_t(new Integer(-1)),
                                            exprptr_t(new Integer(1)),
                                            false,
                                            false))) {}

    objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1 || !isinstance<Expression>(args[0]))
            throw std::runtime_error("[Cos]超出定义域");
        return Cos(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr()).simplify();
    }
};