#pragma once

#include "UndefinedFunction.h"
#include "Set.h"

class SetUndefFunction : public UndefinedFunction, public Set
{
public:
    SetUndefFunction(std::string name, funcargs_t args,
                     setptr_t domain = setptr_t(new UniversalSet()),
                     setptr_t range = setptr_t(new UniversalSet()))
        : UndefinedFunction(name, args, domain, range) {}

    virtual Object *copyThis() { return new SetUndefFunction(*this); }

    virtual boolptr_t operator==(objptr_t b) { return UndefinedFunction::operator==(b); }
};