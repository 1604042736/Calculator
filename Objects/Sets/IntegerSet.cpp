#include "IntegerSet.h"
#include "Common.h"

boolptr_t IntegerSet::contains(objptr_t b)
{
    if (isinstance<Integer>(b))
        return to_boolean(true);
    return Set::contains(b);
}