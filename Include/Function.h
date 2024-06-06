#pragma once

#include "Object.h"
#include "Set.h"
#include "UniversalSet.h"

typedef std::vector<objptr_t> funcargs_t;

/*函数*/
class Function : virtual public Object
{
public:
    Function(setptr_t domain = setptr_t(new UniversalSet()),
             setptr_t range = setptr_t(new UniversalSet()));

    virtual boolptr_t inDomain(funcargs_t);

    setptr_t domain; // 作用域
    setptr_t range;  // 值域
};

typedef std::shared_ptr<Function> funcptr_t;