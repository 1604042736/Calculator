#include "False.h"
#include "True.h"

boolptr_t False::operator&&(boolptr_t)
{
    return boolptr_t(new False());
}

boolptr_t False::operator!()
{
    return boolptr_t(new True());
}

boolptr_t False::operator==(boolptr_t b)
{
    return to_boolean(isinstance<False>(b));
}