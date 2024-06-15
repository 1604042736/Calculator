#pragma once

#include "Expression.h"
#include "Function.h"
#include "UniversalSet.h"

/*可以作为表达式的函数*/
class ExprFunction : virtual public Function, virtual public Expression
{
public:
    ExprFunction(setptr_t domain = setptr_t(new UniversalSet()), setptr_t range = setptr_t(new RealSet()))
        : Function(domain, range)
    {
        // 这里调用父类构造函数似乎没有更新domain和range(由SArgExprFunction和MArgExprFunction调用该构造函数)
        this->domain = domain;
        this->range = range;
    }
};

class SArgExprFunction : public SArgFunction_T<ExprFunction, Expression>
{
public:
    SArgExprFunction(std::string name,
                     exprptr_t arg,
                     setptr_t domain = setptr_t(new UniversalSet()),
                     setptr_t range = setptr_t(new RealSet()))
        : SArgFunction_T<ExprFunction, Expression>(name, arg, domain, range)
    {
        this->domain = domain;
        this->range = range;
    }

    virtual exprptr_t diff(exprptr_t target)
    {
        return Expression::diff(target) * this->arg->diff(target);
    }
};

class MArgExprFunction : public MArgFunction_T<ExprFunction, Expression>
{
public:
    MArgExprFunction(std::string name,
                     std::vector<exprptr_t> args,
                     setptr_t domain = setptr_t(new UniversalSet()),
                     setptr_t range = setptr_t(new RealSet()))
        : MArgFunction_T<ExprFunction, Expression>(name, args, domain, range)
    {
    }
};