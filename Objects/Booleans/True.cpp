#include "True.h"
#include "False.h"

boolptr_t True::operator||(boolptr_t)
{
    return boolptr_t(new True());
}

boolptr_t True::operator!()
{
    return boolptr_t(new False());
}

boolptr_t True::operator==(boolptr_t b)
{
    return to_boolean(isinstance<True>(b));
}