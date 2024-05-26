#include "EmptySet.h"
#include "False.h"

boolptr_t EmptySet::contains(objptr_t)
{
    // 空集不包含任何元素
    return boolptr_t(new False());
}

boolptr_t EmptySet::includes(setptr_t)
{
    return boolptr_t(new False());
}