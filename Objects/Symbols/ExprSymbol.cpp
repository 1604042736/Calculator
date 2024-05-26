#include "ExprSymbol.h"
#include "Mul.h"
#include "Integer.h"
#include "Pow.h"
#include "Common.h"
#include "Derivative.h"

exprptr_t ExprSymbol::operator+(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) + *(ExprSymbol *)_1.get();
    return Expression::operator+(_1);
}
exprptr_t ExprSymbol::operator-(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) - *(ExprSymbol *)_1.get();
    return Expression::operator-(_1);
}
exprptr_t ExprSymbol::operator*(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) * *(ExprSymbol *)_1.get();
    return Expression::operator*(_1);
}
exprptr_t ExprSymbol::operator/(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return (*this) / *(ExprSymbol *)_1.get();
    return Expression::operator/(_1);
}
boolptr_t ExprSymbol::operator==(exprptr_t _1)
{
    if (isinstance<ExprSymbol>(_1))
        return to_boolean((*this) == *(ExprSymbol *)_1.get());
    return Expression::operator==(_1);
}

exprptr_t ExprSymbol::operator+(ExprSymbol b)
{
    if ((*this) == b) // a + a = 2 * a
        return exprptr_t(new Mul({exprptr_t(new Integer(2)),
                                  this->copyToExprPtr()}));
    return Expression::operator+(exprptr_t(dynamic_cast<Expression *>(b.copyThis())));
}

exprptr_t ExprSymbol::operator-(ExprSymbol b)
{
    if ((*this) == b) // a - a = 0
        return exprptr_t(new Integer(0));
    return Expression::operator-(exprptr_t(dynamic_cast<Expression *>(b.copyThis())));
}

exprptr_t ExprSymbol::operator*(ExprSymbol b)
{
    if ((*this) == b) // a * a = a ^ 2
        return this->pow(Integer(2));
    return Expression::operator*(exprptr_t(dynamic_cast<Expression *>(b.copyThis())));
}

exprptr_t ExprSymbol::operator/(ExprSymbol b)
{
    if ((*this) == b) // a / a = 1
        return exprptr_t(new Integer(1));
    return Expression::operator/(exprptr_t(dynamic_cast<Expression *>(b.copyThis())));
}

Integer ExprSymbol::getWeight()
{
    return Integer(std::hash<std::string>()(this->name));
}

exprptr_t ExprSymbol::diff(exprptr_t target)
{
    exprptr_t result = Expression::diff(target);
    if (isinstance<Derivative>(result))
        // 看成常量
        return exprptr_t(new Integer(0));
    return result;
}