#include <iostream>

#include "Mul.h"
#include "Integer.h"
#include "Reciprocal.h"
#include "Add.h"

#include "Derivative.h"
#include "ExprSymbol.h"

exprptr_t Mul::operator+(Mul b)
{
    // a*c+b*c=(a+b)*c
    exprptr_t same = nullptr; // 相同的参数
    // 不同的参数
    expropargs_t adiff(this->args);
    expropargs_t bdiff(b.args);

    for (size_t i = 0; i < adiff.size();)
    {
        bool flag = false;
        for (size_t j = 0; j < bdiff.size(); j++)
        {
            if (isinstance<True>(adiff[i] == bdiff[j]))
            {
                if (same == nullptr)
                    same = adiff[i];
                else
                    same = same * adiff[i];
                adiff.erase(adiff.begin() + i);
                bdiff.erase(bdiff.begin() + j);
                flag = true;
                break;
            }
        }
        if (!flag)
            i++;
    }
    exprptr_t a;
    if (adiff.size() == 0)
        a = exprptr_t(new Integer(1));
    else
    {
        a = adiff[0];
        for (size_t i = 1; i < adiff.size(); i++)
            a = a * adiff[i];
    }
    exprptr_t c;
    if (bdiff.size() == 0)
        c = exprptr_t(new Integer(1));
    else
    {
        c = bdiff[0];
        for (size_t i = 1; i < bdiff.size(); i++)
            c = c * bdiff[i];
    }
    if (same != nullptr)
    {
        exprptr_t sum = a + c;
        if (!isinstance<Add>(sum))
            return same * sum;
    }
    return ExprOp::operator+(b.copyToExprPtr());
}

exprptr_t Mul::operator+(exprptr_t b)
{
    if (isinstance<Mul>(b))
        return *this + *(Mul *)b.get();
    else
    {
        exprptr_t t = *this + Mul({b, exprptr_t(new Integer(1))});
        if (isinstance<Add>(t))
            t = Expression::operator+(b);
        return t;
    }
}

exprptr_t Mul::operator*(Mul b)
{
    exprptr_t result(dynamic_cast<Expression *>(this->copyThis()));
    for (size_t i = 0; i < b.args.size(); i++)
        result = result * b.args[i];
    return result;
}

exprptr_t Mul::operator*(exprptr_t b)
{
    if (isinstance<Mul>(b))
        return (*this) * *(Mul *)b.get();
    if (isinstance<Add>(b))
        return b * *this;
    bool flag = false;
    expropargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        exprptr_t t = this->args[i] * b;
        if (flag || isinstance<Mul>(t)) // 之前已经合并过或者不可以合并
            args.push_back(this->args[i]);
        else
        {
            flag = true;
            if (isinstance<True>(t == Integer(0)))
                return exprptr_t(new Integer(0));
            args.push_back(t);
        }
    }
    if (!flag)
        args.push_back(b);
    return exprptr_t(new Mul(args));
}

exprptr_t Mul::reciprocal()
{
    exprptr_t result = this->args[0]->reciprocal();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result * this->args[i]->reciprocal();
    return result;
}

exprptr_t Mul::diff(exprptr_t target)
{
    exprptr_t result = Expression::diff(target);
    if (!isinstance<Derivative>(result))
        return result;
    if (isinstance<Symbol>(target))
    {
        for (size_t i = 0; i < this->args.size(); i++)
        {
            expropargs_t args;
            for (size_t j = 0; j < i; j++)
                args.push_back(this->args[j]);
            args.push_back(this->args[i]->diff(target));
            for (size_t j = i + 1; j < this->args.size(); j++)
                args.push_back(this->args[j]);
            if (i == 0)
                result = Mul(args).simplify();
            else
                result = result + Mul(args).simplify();
        }
    }
    return result;
}

exprptr_t Mul::opposite()
{
    exprptr_t result = nullptr;
    bool has__1 = false;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (!has__1 && isinstance<True>(this->args[i] == Integer(-1)))
            has__1 = true;
        else
        {
            if (result == nullptr)
                result = this->args[i];
            else
                result = result * this->args[i];
        }
    }
    if (has__1)
        return result;
    return result * Integer(-1);
}

exprptr_t Mul::getCoef(exprptr_t target)
{
    if (isinstance<True>(this->operator==(target)))
        return exprptr_t(new Integer(1));
    exprptr_t coef = nullptr;
    bool has_coef = false;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (isinstance<True>(this->args[i] == target))
        {
            if (!has_coef)
                has_coef = true;
            else // 防止有两个相同的对象
                return exprptr_t(new Integer(0));
        }
        else
        {
            if (coef == nullptr)
                coef = this->args[i];
            else
                coef = coef * this->args[i];
        }
    }
    if (has_coef)
        return coef;
    return exprptr_t(new Integer(0));
}

/*获取分子*/
exprptr_t Mul::getNume()
{
    expropargs_t result;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (typeid(*this->args[i].get()) != typeid(Reciprocal))
            result.push_back(this->args[i]);
    }
    if (result.size() == 0)
        return exprptr_t(new Integer(1));
    if (result.size() == 1)
        return result[0];
    return exprptr_t(new Mul(result));
}

/*获取分母*/
exprptr_t Mul::getDeno()
{
    expropargs_t result;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (isinstance<Reciprocal>(this->args[i]))
            result.push_back(((Reciprocal *)this->args[i].get())->reciprocal());
    }
    if (result.size() == 0)
        return nullptr;
    if (result.size() == 1)
        return result[0];
    return exprptr_t(new Mul(result));
}

std::string Mul::toString()
{
    exprptr_t nume = this->getNume();
    exprptr_t deno = this->getDeno();
    if (deno != nullptr)
        return nume->toString() + "/" + deno->toString();
    else
        return ExprOp::toString();
}

prettystring_t Mul::toPrettyString()
{
    exprptr_t nume = this->getNume();
    exprptr_t deno = this->getDeno();
    if (deno == nullptr)
        return ExprOp::toPrettyString();

    prettystring_t nume_str = nume->toPrettyString();
    prettystring_t deno_str = deno->toPrettyString();
    size_t max_len = std::max(nume_str[0].size(), deno_str[0].size()) + 2;

    prettystring_t result;
    for (size_t i = 0; i < nume_str.size(); i++)
        result.push_back(std::string((max_len - nume_str[i].size()) / 2, ' ') + nume_str[i]);
    result.push_back(std::string(max_len, '-'));
    for (size_t i = 0; i < deno_str.size(); i++)
        result.push_back(std::string((max_len - deno_str[i].size()) / 2, ' ') + deno_str[i]);

    return normalize(result);
}

std::string Mul::toLateX()
{
    exprptr_t nume = this->getNume();
    exprptr_t deno = this->getDeno();
    if (deno == nullptr)
        return ExprOp::toLateX();
    return "\\frac{" + nume->toLateX() + "}{" + deno->toLateX() + "}";
}

exprptr_t Mul::_simplify()
{
    exprptr_t result = this->args[0]->simplify();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result * this->args[i]->simplify();
    return result;
}

exprptr_t Mul::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    exprptr_t result = this->args[0]->eval(keep);
    for (size_t i = 1; i < this->args.size(); i++)
        result = result * this->args[i]->eval(keep);
    return result;
}

Integer Mul::getWeight()
{
    Integer weight = this->args[0]->getWeight();
    for (size_t i = 1; i < this->args.size(); i++)
        weight = weight * this->args[i]->getWeight();
    return weight;
}

setptr_t Mul::belongto()
{
    setptr_t result = this->args[0]->belongto();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result->mul(this->args[i]->belongto());
    return result;
}