#include <iostream>

#include "Add.h"
#include "Integer.h"
#include "Opposite.h"
#include "Pow.h"
#include "Common.h"
#include "Derivative.h"

std::string Add::toString()
{
    std::string result = "";
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (i > 0 && !isinstance<Opposite>(this->args[i]))
            result += " + ";
        if (this->args[i]->getPriority() < this->getPriority())
            result += " ( " + this->args[i]->toString() + " ) ";
        else
            result += this->args[i]->toString();
    }
    return result;
}

prettystring_t Add::toPrettyString()
{
    std::vector<prettystring_t> args_str;
    size_t max_height = 1;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        prettystring_t s = this->args[i]->toPrettyString();
        args_str.push_back(s);
        max_height = std::max(max_height, s.size());
    }

    prettystring_t result(max_height, "");
    for (size_t i = 0; i < args_str.size(); i++)
    {
        size_t y = (max_height - args_str[i].size()) / 2; // 居中
        if (isinstance<Pow>(this->args[i]))
        {
            // Pow与加号对齐
            if ((max_height - 1) / 2 < args_str[i].size() - 1)
            {
                size_t d = (args_str[i].size() - 1) - (max_height - 1) / 2;
                for (size_t j = 0; j < d; j++)
                    result.insert(result.begin(), std::string(result.back().size(), ' '));
                max_height += d;
            }
            y = (max_height - 1) / 2 - (args_str[i].size() - 1);
        }

        // 如果不是Opposite就在前面加上+号
        if (i > 0 && !isinstance<Opposite>(this->args[i]))
        {
            result[(max_height - 1) / 2] += " + ";
            result = normalize(result);
        }

        bool flag = this->args[i]->getPriority() < this->getPriority();

        if (flag)
        {
            for (size_t j = 0; j < result.size(); j++)
                result[j] += " ( ";
        }

        for (size_t j = 0; j < args_str[i].size(); j++, y++)
            result[y] += args_str[i][j];

        if (flag)
        {
            for (size_t j = 0; j < result.size(); j++)
                result[j] += " ) ";
        }
    }

    return normalize(result);
}

exprptr_t Add::operator+(Add b)
{
    exprptr_t result(dynamic_cast<Expression *>(this->copyThis()));
    for (size_t i = 0; i < b.args.size(); i++)
        result = result + b.args[i];
    return result;
}

exprptr_t Add::operator+(exprptr_t b)
{
    if (isinstance<Add>(b))
        return (*this) + *(Add *)b.get();
    bool flag = false;
    expropargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        exprptr_t t = this->args[i] + b;
        if (flag || isinstance<Add>(t)) // 之前已经合并过或者不可以合并
            args.push_back(this->args[i]);
        else
        {
            flag = true;
            if (!isinstance<True>(t == Integer(0)))
                args.push_back(t);
        }
    }
    if (!flag)
        args.push_back(b);
    if (args.size() == 1)
        return args[0];
    return exprptr_t(new Add(args));
}

exprptr_t Add::operator*(exprptr_t b)
{
    // 分配律
    exprptr_t result = this->args[0] * b;
    for (size_t i = 1; i < this->args.size(); i++)
        result = result + this->args[i] * b;
    return result;
}

exprptr_t Add::opposite()
{
    expropargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
        args.push_back(this->args[i]->opposite());
    return exprptr_t(new Add(args));

    exprptr_t result = this->args[0]->opposite();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result + this->args[i]->opposite();
    return result;
}

exprptr_t Add::pow(Integer exp)
{
    Integer b(exp);
    b.sign = SIGN_POSITIVE;
    if (b == 0)
        return exprptr_t(new Integer(1));
    b = b - 1;
    exprptr_t x(dynamic_cast<Expression *>(this->copyThis()));
    exprptr_t result(dynamic_cast<Expression *>(this->copyThis()));
    while (b > 0)
    {
        result = result * x;
        b = b - 1;
    }
    if (exp >= 0)
        return result;
    else
        return result->reciprocal();
}

exprptr_t Add::pow(exprptr_t _1)
{
    if (isinstance<Integer>(_1))
        return this->pow(*dynamic_cast<Integer *>(_1.get()));
    return Expression::pow(_1);
}

exprptr_t Add::diff(exprptr_t target)
{
    exprptr_t result = Expression::diff(target);
    if (!isinstance<Derivative>(result))
        return result;
    result = this->args[0]->diff(target);
    for (size_t i = 1; i < this->args.size(); i++)
        result = result + this->args[i]->diff(target);
    return result;
}

exprptr_t Add::getCoef(exprptr_t target)
{
    if (isinstance<True>(this->operator==(target)))
        return exprptr_t(new Integer(1));
    exprptr_t coef = this->args[0]->getCoef(target);
    for (size_t i = 1; i < this->args.size(); i++)
        coef = coef + this->args[i]->getCoef(target);
    return coef;
}

exprptr_t Add::_simplify()
{
    exprptr_t result = this->args[0]->simplify();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result + this->args[i]->simplify();
    return result;
}

exprptr_t Add::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    exprptr_t result = this->args[0]->eval(keep);
    for (size_t i = 1; i < this->args.size(); i++)
        result = result + this->args[i]->eval(keep);
    return result;
}

Integer Add::getWeight()
{
    Integer weight = this->args[0]->getWeight();
    for (size_t i = 1; i < this->args.size(); i++)
        weight = weight + this->args[i]->getWeight();
    return weight;
}

setptr_t Add::belongto()
{
    setptr_t result = this->args[0]->belongto();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result->add(this->args[i]->belongto());
    return result;
}