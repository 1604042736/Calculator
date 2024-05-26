#include "UniversalSet.h"
#include "True.h"

boolptr_t UniversalSet::contains(objptr_t)
{
    // 全集包含所有元素
    return boolptr_t(new True());
}

boolptr_t UniversalSet::includes(setptr_t)
{
    return boolptr_t(new True());
}