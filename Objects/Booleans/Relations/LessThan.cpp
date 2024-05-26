#include "LessThan.h"
#include "StrictGreaterThan.h"

boolptr_t LessThan::operator!()
{
    return boolptr_t(new StrictGreaterThan(this->lhs, this->rhs));
}