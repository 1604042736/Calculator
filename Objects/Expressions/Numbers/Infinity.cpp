#include "Infinity.h"
#include "Integer.h"

#include "Mul.h"
#include "Add.h"

exprptr_t Infinity::operator+(exprptr_t b)
{
    if (!isinstance<Infinity>(b))
        return this->copyToExprPtr();
    Infinity *t = dynamic_cast<Infinity *>(b.get());
    if (t->sign == this->sign)
        return this->copyToExprPtr();
    return exprptr_t(new Integer(0));
}

exprptr_t Infinity::operator*(exprptr_t b)
{
    if (isinstance<True>(b > Integer(0)))
        return this->copyToExprPtr();
    else if (isinstance<True>(b == Integer(0)))
        return exprptr_t(new Integer(0));
    else if (isinstance<True>(b < Integer(0)))
        return this->opposite();
    return Number::operator*(b);
}

exprptr_t Infinity::pow(exprptr_t b)
{
    if (isinstance<True>(b > Integer(0)))
        return this->copyToExprPtr();
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
    if (isinstance<Infinity>(b) && dynamic_cast<Infinity *>(b.get())->sign == this->sign)
        return boolptr_t(new True());
    return boolptr_t(new False());
}