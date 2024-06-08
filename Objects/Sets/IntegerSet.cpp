#include "IntegerSet.h"
#include "Common.h"

boolptr_t IntegerSet::contains(objptr_t b)
{
    if (isinstance<Integer>(b))
        return to_boolean(true);
    else if (isinstance<Number>(b))
        return to_boolean(false);
    return Set::contains(b);
}