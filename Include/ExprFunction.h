#pragma once

#include "Expression.h"
#include "Function.h"
#include "UniversalSet.h"

/*可以作为表达式的函数*/
class ExprFunction : virtual public Function, virtual public Expression
{
public:
    ExprFunction(setptr_t domain = setptr_t(new UniversalSet()), setptr_t range = setptr_t(new RealSet()))
        : Function(domain, range) {}
};

class SArgExprFunction : public SArgFunction<ExprFunction, Expression>
{
public:
    SArgExprFunction(std::string name,
                     exprptr_t arg,
                     setptr_t domain = setptr_t(new UniversalSet()),
                     setptr_t range = setptr_t(new RealSet()))
        : SArgFunction<ExprFunction, Expression>(name, arg, domain, range) {}

    virtual exprptr_t diff(exprptr_t target)
    {
        return Expression::diff(target) * this->arg->diff(target);
    }
};