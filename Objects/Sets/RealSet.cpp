#include "RealSet.h"
#include "Expression.h"

boolptr_t RealSet::contains(objptr_t b)
{
    if (isinstance<Expression>(b))
        return to_boolean(true);
    return to_boolean(false);
}

boolptr_t RealSet::includes(setptr_t subset)
{
    if (isinstance<IntegerSet>(subset) || isinstance<RationalSet>(subset))
        return boolptr_t(new True());
    return Set::includes(subset);
}