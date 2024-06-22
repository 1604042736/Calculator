#include <iostream>

#include "Boolean.h"
#include "Not.h"
#include "And.h"
#include "Or.h"
#include "True.h"
#include "Equality.h"
#include "False.h"

boolptr_t Boolean::operator&&(boolptr_t b)
{
    if (isinstance<False>(this) || isinstance<False>(b))
        return boolptr_t(new False());
    if (isinstance<True>(this))
        return b;
    if (isinstance<True>(b))
        return this->copyToBoolPtr();
    return boolptr_t(new And({this->copyToBoolPtr(), b}));
}
boolptr_t Boolean::operator&&(Boolean &b)
{
    return this->operator&&(b.copyToBoolPtr());
}
boolptr_t Boolean::operator&&(Boolean &&b)
{
    return this->operator&&(b.copyToBoolPtr());
}
boolptr_t operator&&(boolptr_t a, boolptr_t b) { return a->operator&&(b); }
boolptr_t operator&&(boolptr_t a, Boolean &b) { return a->operator&&(b); }
boolptr_t operator&&(boolptr_t a, Boolean &&b) { return a->operator&&(b); }

boolptr_t Boolean::operator||(boolptr_t b)
{
    if (isinstance<True>(this) || isinstance<True>(b))
        return boolptr_t(new True());
    if (isinstance<False>(this))
        return b;
    if (isinstance<False>(b))
        return this->copyToBoolPtr();
    return boolptr_t(new Or({this->copyToBoolPtr(), b}));
}
boolptr_t Boolean::operator||(Boolean &b)
{
    return this->operator||(this->copyToBoolPtr());
}
boolptr_t Boolean::operator||(Boolean &&b)
{
    return this->operator||(this->copyToBoolPtr());
}
boolptr_t operator||(boolptr_t a, boolptr_t b) { return a->operator||(b); }
boolptr_t operator||(boolptr_t a, Boolean &b) { return a->operator||(b); }
boolptr_t operator||(boolptr_t a, Boolean &&b) { return a->operator||(b); }

boolptr_t Boolean::operator!()
{
    return boolptr_t(new Not(this->copyToBoolPtr()));
}

boolptr_t Boolean::operator==(boolptr_t _1) { return boolptr_t(new Equality(this->copyToBoolPtr(), _1)); }
boolptr_t Boolean::operator==(Boolean &_1)
{
    return this->operator==(_1.copyToBoolPtr());
}
boolptr_t Boolean::operator==(Boolean &&_1)
{
    return this->operator==(_1.copyToBoolPtr());
}
boolptr_t operator==(boolptr_t _1, boolptr_t _2) { return _1->operator==(_2); }
boolptr_t operator==(boolptr_t _1, Boolean &_2) { return _1->operator==(_2); }
boolptr_t operator==(boolptr_t _1, Boolean &&_2) { return _1->operator==(_2); }
boolptr_t Boolean::operator!=(boolptr_t _1) { return !(this->operator==(_1)); }
boolptr_t Boolean::operator!=(Boolean &_1)
{
    return this->operator!=(_1.copyToBoolPtr());
}
boolptr_t Boolean::operator!=(Boolean &&_1)
{
    return this->operator!=(_1.copyToBoolPtr());
}
boolptr_t operator!=(boolptr_t _1, boolptr_t _2) { return _1->operator!=(_2); }
boolptr_t operator!=(boolptr_t _1, Boolean &_2) { return _1->operator!=(_2); }
boolptr_t operator!=(boolptr_t _1, Boolean &&_2) { return _1->operator!=(_2); }

boolptr_t Boolean::operator&&(objptr_t b)
{
    if (isinstance<Boolean>(b))
        return this->operator&&(dynamic_cast<Boolean *>(b.get())->copyToBoolPtr());
    return Object::operator&&(b);
}

boolptr_t Boolean::operator||(objptr_t b)
{
    if (isinstance<Boolean>(b))
        return this->operator||(dynamic_cast<Boolean *>(b.get())->copyToBoolPtr());
    return Object::operator||(b);
}

boolptr_t Boolean::simplify()
{
    boolptr_t result = this->_simplify();
    boolptr_t pre;
    do
    {
        pre = result;
        result = result->_simplify();
    } while (isinstance<True>(result != pre));

    return result;
}

boolptr_t operator!(boolptr_t a)
{
    return a->operator!();
}

boolptr_t to_boolean(bool a)
{
    if (a)
        return boolptr_t(new True());
    else
        return boolptr_t(new False());
}