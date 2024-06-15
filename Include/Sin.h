#pragma once

#include "TrigFunction.h"
#include "ExprSymbol.h"
#include "ExprMapping.h"
#include "RealSet.h"
#include "Interval.h"
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

class SinMapping : public ExprMapping
{
public:
    SinMapping()
        : ExprMapping("sin",
                      {},
                      setptr_t(new RealSet()),
                      setptr_t(new Interval(exprptr_t(new Integer(-1)),
                                            exprptr_t(new Integer(1)),
                                            false,
                                            false))) {}

    objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1 || !isinstance<Expression>(args[0]))
            throw std::runtime_error("[Sin]超出定义域");
        return Sin(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr()).simplify();
    }
};