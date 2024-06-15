#include "RationalSet.h"

#include "Integer.h"
#include "Rational.h"
#include "Float.h"

boolptr_t RationalSet::contains(objptr_t b)
{
    if (isinstance<Integer>(b) || isinstance<Rational>(b) || isinstance<Float>(b))
        return to_boolean(true);
    return Set::contains(b);
}