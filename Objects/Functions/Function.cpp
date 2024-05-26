#include "Function.h"

Function::Function(setptr_t domain, setptr_t range)
{
    // 默认定义域和值域都是全集
    this->domain = domain;
    this->range = range;
}