#include <algorithm>

#include "ExprSymbol.h"
#include "Mul.h"
#include "Integer.h"
#include "Pow.h"

#include "Derivative.h"
#include "Number.h"

ExprSymbol::ExprSymbol(std::string name, setptr_t range) : range(range), Symbol(name)
{
}

exprptr_t ExprSymbol::operator+(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) + dynamic_cast<ExprSymbol *>(_1.get());
    return Expression::operator+(_1);
}
exprptr_t ExprSymbol::operator-(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) - dynamic_cast<ExprSymbol *>(_1.get());
    return Expression::operator-(_1);
}
exprptr_t ExprSymbol::operator*(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) * dynamic_cast<ExprSymbol *>(_1.get());
    return Expression::operator*(_1);
}
exprptr_t ExprSymbol::operator/(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) / dynamic_cast<ExprSymbol *>(_1.get());
    return Expression::operator/(_1);
}
boolptr_t ExprSymbol::operator==(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) == dynamic_cast<ExprSymbol *>(_1.get());
    return Expression::operator==(_1);
}

exprptr_t ExprSymbol::operator+(ExprSymbol *b)
{
    if (isinstance<True>(this->operator==(b))) // a + a = 2 * a
        return exprptr_t(new Mul({exprptr_t(new Integer(2)),
                                  this->copyToExprPtr()}));
    return Expression::operator+(b->copyToExprPtr());
}

exprptr_t ExprSymbol::operator-(ExprSymbol *b)
{
    if (isinstance<True>(this->operator==(b))) // a - a = 0
        return exprptr_t(new Integer(0));
    return Expression::operator-(b->copyToExprPtr());
}

exprptr_t ExprSymbol::operator*(ExprSymbol *b)
{
    if (isinstance<True>(this->operator==(b))) // a * a = a ^ 2
        return this->pow(Integer(2));
    return Expression::operator*(b->copyToExprPtr());
}

exprptr_t ExprSymbol::operator/(ExprSymbol *b)
{
    if (isinstance<True>(this->operator==(b))) // a / a = 1
        return exprptr_t(new Integer(1));
    return Expression::operator/(b->copyToExprPtr());
}

boolptr_t ExprSymbol::operator==(ExprSymbol *b)
{
    bool c = this->name == b->name;
    if (c || (isinstance<Number>(this) && isinstance<Number>(b)))
        return to_boolean(c);
    return Expression::operator==(b->copyToExprPtr());
}

Integer ExprSymbol::getWeight()
{
    return std::distance(names.begin(), std::find(names.begin(), names.end(), this->name));
}