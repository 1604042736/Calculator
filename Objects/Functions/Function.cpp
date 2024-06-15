#include "Function.h"
#include "Boolean.h"

#include "False.h"
#include "Tuple.h"

Function::Function(setptr_t domain, setptr_t range)
{
    // 默认定义域和值域都是全集
    this->domain = domain;
    this->range = range;
}

boolptr_t Function::inDomain(funcargs_t args)
{
    if (args.size() == 1)
        return this->domain->contains(args[0]);
    objptr_t t(new Tuple({args[0], args[1]}));
    for (size_t i = 2; i < args.size(); i++)
        t = objptr_t(new Tuple({t, args[i]}));
    return this->domain->contains(t);
}