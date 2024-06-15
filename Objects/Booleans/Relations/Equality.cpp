#include "Equality.h"
#include "Unequality.h"
#include "GreaterThan.h"
#include "LessThan.h"

boolptr_t Equality::operator||(StrictGreaterThan b)
{
    if (this->lhs == b.lhs && this->rhs == b.rhs)
        return boolptr_t(new GreaterThan(this->lhs, this->rhs));
    return Relation::operator||(b.copyToBoolPtr());
}

boolptr_t Equality::operator||(StrictLessThan b)
{
    if (this->lhs == b.lhs && this->rhs == b.rhs)
        return boolptr_t(new LessThan(this->lhs, this->rhs));
    return Relation::operator||(b.copyToBoolPtr());
}

boolptr_t Equality::operator||(boolptr_t b)
{
    if (isinstance<StrictGreaterThan>(b))
        return (*this) || *(StrictGreaterThan *)b.get();
    if (isinstance<StrictLessThan>(b))
        return (*this) || *(StrictLessThan *)b.get();
    return Relation::operator||(b);
}

boolptr_t Equality::operator!()
{
    return boolptr_t(new Unequality(this->lhs, this->rhs));
}

boolptr_t Equality::_simplify()
{
    return this->lhs->simplify() == this->rhs->simplify();
}