#include "GreaterThan.h"
#include "StrictLessThan.h"

boolptr_t GreaterThan::operator!()
{
    return boolptr_t(new StrictLessThan(this->lhs, this->rhs));
}