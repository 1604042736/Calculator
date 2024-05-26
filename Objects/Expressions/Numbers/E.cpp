#include "E.h"
#include "Integer.h"
#include "Rational.h"
#include "Common.h"

exprptr_t E::operator+(exprptr_t _1)
{
    if (isinstance<E>(_1))
        return ((*this) + *(E *)_1.get()).copyToExprPtr();
    return Number::operator+(_1);
}
exprptr_t E::operator-(exprptr_t _1)
{
    if (isinstance<E>(_1))
        return ((*this) - *(E *)_1.get()).copyToExprPtr();
    return Number::operator-(_1);
}
exprptr_t E::operator*(exprptr_t _1)
{
    if (isinstance<E>(_1))
        return ((*this) * *(E *)_1.get()).copyToExprPtr();
    return Number::operator*(_1);
}
exprptr_t E::operator/(exprptr_t _1)
{
    if (isinstance<E>(_1))
        return ((*this) / *(E *)_1.get()).copyToExprPtr();
    return Number::operator/(_1);
}

boolptr_t E::operator==(exprptr_t _1)
{
    if (typeid(*_1.get()) == typeid(E))
        return to_boolean((*this) == *(E *)_1.get());
    return Number::operator==(_1);
}

exprptr_t E::eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    Rational s(0), a(1);
    Integer keep_2 = keep * 2;
    for (Integer i = 1; i < keep_2; i = i + 1)
    {
        s = s + (Integer)1 / a;
        a = a * i;
    }
    return s.eval(keep);
}