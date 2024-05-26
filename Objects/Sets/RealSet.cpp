#include "RealSet.h"
#include "Common.h"

boolptr_t RealSet::includes(setptr_t subset)
{
    if (isinstance<IntegerSet>(subset) || isinstance<RationalSet>(subset))
        return boolptr_t(new True());
    return Set::includes(subset);
}