#include "Infinity.h"
#include "Integer.h"
#include "Common.h"
#include "Mul.h"
#include "Add.h"

exprptr_t Infinity::operator+(exprptr_t b)
{
    // 1*oo + 1*b 这样如果b中含有oo就会进行合并
    exprptr_t t = Mul({this->copyToExprPtr()}) + b;

    if (isinstance<Add>(t))
        t = exprptr_t(new Infinity());
    return t;
}

exprptr_t Infinity::operator*(exprptr_t b)
{
    if (isinstance<True>(b > Integer(0)))
        return exprptr_t(new Infinity());
    else if (isinstance<True>(b == Integer(0)))
        return exprptr_t(new Integer(0));
    else if (isinstance<True>(b < Integer(0)))
        return Infinity().opposite();
    return Number::operator*(b);
}

exprptr_t Infinity::pow(exprptr_t b)
{
    if (isinstance<True>(b > Integer(0)))
        return exprptr_t(new Infinity());
    else if (isinstance<True>(b == Integer(0)))
        return exprptr_t(new Integer(1));
    else if (isinstance<True>(b < Integer(0)))
        return exprptr_t(new Integer(0));
    return Number::pow(b);
}

exprptr_t Infinity::reciprocal()
{
    return exprptr_t(new Integer(0));
}

boolptr_t Infinity::operator==(exprptr_t b)
{
    if (isinstance<Infinity>(b))
        return boolptr_t(new True());
    return boolptr_t(new False());
}