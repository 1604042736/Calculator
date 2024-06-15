#pragma once

#include "Set.h"
#include "Function.h"
#include "UniversalSet.h"

class SetFunction : virtual public Function, virtual public Set
{
public:
    SetFunction(setptr_t domain = setptr_t(new UniversalSet()), setptr_t range = setptr_t(new UniversalSet()))
        : Function(domain, range)
    {
        this->domain = domain;
        this->range = range;
    }
};
typedef SArgFunction_T<SetFunction, Set> SArgSetFunction;
typedef MArgFunction_T<SetFunction, Set> MArgSetFunction;