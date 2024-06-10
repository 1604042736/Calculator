#include "True.h"
#include "False.h"
#include "Common.h"

boolptr_t True::operator||(boolptr_t)
{
    return boolptr_t(new True());
}

boolptr_t True::operator!()
{
    return boolptr_t(new False());
}

bool True::operator==(boolptr_t b)
{
    return isinstance<True>(b);
}