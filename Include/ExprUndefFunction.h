#pragma once

#include "UndefinedFunction.h"
#include "Expression.h"

class ExprUndefFunction : public UndefinedFunction, public Expression
{
public:
    ExprUndefFunction(std::string name, funcargs_t args,
                      setptr_t domain = setptr_t(new UniversalSet()),
                      setptr_t range = setptr_t(new RealSet()))
        : UndefinedFunction(name, args, domain, range) {}

    virtual Object *copyThis() { return new ExprUndefFunction(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new ExprUndefFunction(*this)); }
};