#include <stdexcept>
#include <iostream>

#include "Common.h"
#include "DefinedFunction.h"
#include "Mul.h"
#include "Pow.h"
#include "IntegerSet.h"
#include "Interval.h"
#include "Infinity.h"

objptr_t SimplifyMapping::operator()(funcargs_t args)
{
    if (args.size() != 1)
        throw std::runtime_error("超出定义域");
    return simplify(args[0]);
}

objptr_t EvalMapping::operator()(funcargs_t args)
{
    if (args.size() != 2 || !isinstance<Expression>(args[0]) || !isinstance<Integer>(args[1]))
        throw std::runtime_error("超出定义域");
    return this->operator()(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr(),
                            *dynamic_cast<Integer *>(args[1].get()));
}

objptr_t PrintMapping::operator()(funcargs_t args)
{
    for (size_t i = 0; i < args.size(); i++)
    {
        args[i] = simplify(args[i]);
        print(args[i]->toPrettyString());
        if (isinstance<DefinedFunction>(args[i]))
        {
            DefinedFunction *f = dynamic_cast<DefinedFunction *>(args[i].get());
            std::cout << "||\n";
            std::cout << "\\/\n";
            for (size_t j = 0; j < f->sections->size(); j++)
                print((*f->sections)[j].toPrettyString());
        }
    }
    return nullptr;
}

objptr_t DiffMapping::operator()(funcargs_t args)
{
    if (args.size() < 2)
        throw std::runtime_error("参数不够");
    std::vector<exprptr_t> expr_args;
    for (size_t i = 0; i < args.size(); i++)
    {
        if (!isinstance<Expression>(args[i]))
            throw std::runtime_error("超出定义域");
        expr_args.push_back(dynamic_cast<Expression *>(args[i].get())->copyToExprPtr());
    }
    exprptr_t result = expr_args[0]->diff(expr_args[1]);
    for (size_t i = 2; i < expr_args.size(); i++)
        result = result->diff(expr_args[i]);
    return result;
}

FactorintMapping::FactorintMapping() : ExprMapping("factorint")
{
    this->domain = Interval(exprptr_t(new Integer(2)), exprptr_t(new Infinity()), false, true) & setptr_t(new IntegerSet());
    this->range = this->domain;
}

objptr_t FactorintMapping::operator()(funcargs_t args)
{
    if (args.size() != 1 || !isinstance<Integer>(args[0]))
        throw std::runtime_error("超出定义域");
    Integer n = *dynamic_cast<Integer *>(args[0].get());
    if (n < 2)
        throw std::runtime_error("超出定义域");
    expropargs_t exprop_args;
    for (Integer i = 2; n != 1; i = i + 1)
    {
        Integer t = 0;
        while (n % i == 0)
        {
            n = floordiv(n, i);
            t = t + 1;
        }
        if (t == 1)
            exprop_args.push_back(i.copyToExprPtr());
        else if (t > 1)
            exprop_args.push_back(exprptr_t(new Pow({i.copyToExprPtr(), t.copyToExprPtr()})));
    }
    return exprptr_t(new Mul(exprop_args));
}

objptr_t ProductSetMapping::operator()(funcargs_t args)
{
    setptr_t result = nullptr;
    for (size_t i = 0; i < args.size(); i++)
    {
        if (!isinstance<Set>(args[i]))
            throw std::runtime_error("超出定义域");
        if (result == nullptr)
            result = dynamic_cast<Set *>(args[i].get())->copyToSetPtr();
        else
            result = result->product(dynamic_cast<Set *>(args[i].get())->copyToSetPtr());
    }
    return result;
}