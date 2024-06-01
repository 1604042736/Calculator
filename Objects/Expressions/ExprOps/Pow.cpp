#include "Pow.h"
#include "Integer.h"
#include "Mul.h"
#include "Add.h"
#include "Common.h"
#include "Derivative.h"
#include "Ln.h"

prettystring_t Pow::toPrettyString()
{
    prettystring_t a = this->args[0]->toPrettyString();
    if (this->args[0]->getPriority() < this->getPriority())
    {
        for (size_t i = 0; i < a.size(); i++)
            a[i] = " ( " + a[i] + " ) ";
    }
    for (size_t i = 1; i < this->args.size(); i++)
    {
        prettystring_t b = this->args[i]->toPrettyString();
        /*
        生成:
         b
        a
        */
        prettystring_t c;
        for (size_t j = 0; j < b.size(); j++)
            c.push_back(std::string(a[0].size(), ' ') + b[j]);
        c.insert(c.end(), a.begin(), a.end());
        a = normalize(c);
    }
    return a;
}

std::string Pow::toLateX()
{
    std::string result = this->args[0]->toLateX();
    for (size_t i = 1; i < this->args.size(); i++)
        result += "^{" + this->args[i]->toLateX() + "}";
    return result;
}

exprptr_t Pow::operator*(Pow b)
{
    // a^m*a^n=a^(m+n)
    if (isinstance<True>(this->getBase() == b.getBase()))
    {
        exprptr_t sum = this->getExp() + b.getExp();
        if (typeid(*sum.get()) != typeid(Add)) // 指数可合并
            return this->getBase()->pow(sum);
    }
    return ExprOp::operator*(exprptr_t(dynamic_cast<Expression *>(b.copyThis())));
}

exprptr_t Pow::operator*(exprptr_t b)
{
    if (isinstance<Pow>(b))
        return (*this) * *(Pow *)b.get();
    exprptr_t base = this->getBase();
    exprptr_t exp = this->getExp();
    if (isinstance<True>(base == b)) // 看成b^1
    {
        // a^m*a=a^(m+1)
        exprptr_t sum = exp + Integer(1);
        if (typeid(*sum.get()) != typeid(Add))
            return base->pow(sum);
    }
    return Expression::operator*(b);
}

exprptr_t Pow::pow(exprptr_t b)
{
    //(a^m)^n=a^(m*n)
    exprptr_t e = this->getExp() * b;
    if (!isinstance<Mul>(e)) // 可以合并
        return this->getBase()->pow(e);
    return ExprOp::pow(b);
}

exprptr_t Pow::diff(exprptr_t target)
{
    exprptr_t result = Expression::diff(target);
    if (!isinstance<Derivative>(result))
        return result;
    // (a^b)'=(e^(b*ln(a)))'=(a^b)*(b'*ln(a)+b/a*a')
    exprptr_t a = this->getBase();
    exprptr_t b = this->getExp();
    return a->pow(b) * (b->diff(target) * Ln(a) + b / a * a->diff(target));
}

exprptr_t Pow::_simplify()
{
    exprptr_t result = this->args[0]->simplify();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result->pow(this->args[i]->simplify());
    return result;
}

exprptr_t Pow::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    exprptr_t result = this->args[0]->eval(keep);
    for (size_t i = 1; i < this->args.size(); i++)
        result = result->pow(this->args[i]->eval(keep), keep);
    return result;
}

Integer Pow::getWeight()
{
    Integer weight = this->args[0]->getWeight();
    for (size_t i = 1; i < this->args.size(); i++)
        weight = *(Integer *)weight.pow(this->args[i]->getWeight()).get();
    return weight;
}