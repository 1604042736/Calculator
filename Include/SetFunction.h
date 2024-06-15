#pragma once

#include "Set.h"
#include "Function.h"
#include "Mapping.h"

class SetFunction : public Function, public Set
{
public:
    SetFunction(std::string name, funcargs_t args) : Function(name, args) {}
    SetFunction(mappingptr_t mapping, funcargs_t args) : Function(mapping, args) {}

    virtual Object *copyThis() { return new SetFunction(*this); }

    virtual boolptr_t operator==(setptr_t);
    virtual boolptr_t operator==(objptr_t b) { return Set::operator==(b); }

    virtual objptr_t _simplify_() { return Set::simplify(); }
};