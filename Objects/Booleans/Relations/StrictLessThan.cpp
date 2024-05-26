#include "StrictLessThan.h"
#include "GreaterThan.h"
#include "LessThan.h"
#include "Equality.h"
#include "Common.h"

boolptr_t StrictLessThan::operator||(Equality b)
{
    if (this->lhs == b.lhs && this->rhs == b.rhs)
        return boolptr_t(new LessThan(this->lhs, this->rhs));
    return Relation::operator||(b.copyToBoolPtr());
}

boolptr_t StrictLessThan::operator||(boolptr_t b)
{
    if (isinstance<Equality>(b))
        return (*this) || *(Equality *)b.get();
    return Relation::operator||(b);
}

boolptr_t StrictLessThan::operator!()
{
    return boolptr_t(new GreaterThan(this->lhs, this->rhs));
}