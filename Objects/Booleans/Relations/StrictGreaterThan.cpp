#include "StrictGreaterThan.h"
#include "LessThan.h"
#include "Equality.h"
#include "GreaterThan.h"

boolptr_t StrictGreaterThan::operator||(Equality b)
{
    if (this->lhs == b.lhs && this->rhs == b.rhs)
        return boolptr_t(new GreaterThan(this->lhs, this->rhs));
    return Relation::operator||(b.copyToBoolPtr());
}

boolptr_t StrictGreaterThan::operator||(boolptr_t b)
{
    if (typeid(*b.get()) == typeid(Equality))
        return (*this) || *(Equality *)b.get();
    return Relation::operator||(b);
}

boolptr_t StrictGreaterThan::operator!()
{
    return boolptr_t(new LessThan(this->lhs, this->rhs));
}