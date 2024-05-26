#include "Unequality.h"
#include "Equality.h"

boolptr_t Unequality::operator!()
{
    return boolptr_t(new Equality(this->lhs, this->rhs));
}

boolptr_t Unequality::_simplify()
{
    return this->lhs->simplify() != this->rhs->simplify();
}