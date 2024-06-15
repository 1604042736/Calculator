#pragma once

#include <stdexcept>
#include <tuple>
#include <functional>
#include <vector>

#include "ExprFunction.h"
#include "Interval.h"
#include "Integer.h"
#include "ExprMapping.h"
#include "ExprSymbol.h"

#include "Add.h"
#include "Mul.h"

// 化简结果
typedef std::tuple<exprptr_t, Integer> redres_t;

/*三角函数*/
class TrigFunction : public SArgExprFunction
{
public:
    TrigFunction(std::string name,
                 exprptr_t arg,
                 setptr_t domain = setptr_t(new RealSet()),
                 setptr_t range = setptr_t(new Interval(exprptr_t(new Integer(-1)),
                                                        exprptr_t(new Integer(1)), false, false)))
        : SArgExprFunction(name, arg, domain, range) {}

    virtual std::string toString() { return this->name + "(" + this->arg->toString() + ")"; }

    virtual exprptr_t _simplify();
    virtual void reducePeriod(exprptr_t &, exprptr_t &);

    virtual setptr_t belongto() { return setptr_t(new Interval(exprptr_t(new Integer(-1)), exprptr_t(new Integer(1)), false, false)); }

    virtual exprptr_t matchSpecial();

    Integer period;                                        // 最小正周期(不含pi)
    std::vector<std::pair<exprptr_t, exprptr_t> > special; // 特殊值
};

template <typename FuncT>
class TrigFunctionMapping : public ExprMapping
{
public:
    TrigFunctionMapping() : ExprMapping("???")
    {
        exprptr_t arg(new ExprSymbol("x"));
        FuncT func(arg);
        this->name = func.name;
        this->domain = func.domain;
        this->range = func.range;
    }

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1 || !isinstance<Expression>(args[0]))
            throw std::runtime_error("[TrigFunctionMapping]超出定义域");
        return exprptr_t(new FuncT(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr()))->simplify();
    }
};

/*
是否可以化简
传入pi前的系数
当pi前系数不含pi或者pi前系数不是常数(不包括那些用符号表示的常数, 比如e)时可以化简
*/
static bool can_reduce(exprptr_t coef) { return isinstance<Number>(coef) && !isinstance<ExprSymbol>(coef); }