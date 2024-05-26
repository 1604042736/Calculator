#pragma once

#include "Function.h"

/*映射*/
class Mapping : public Function
{
public:
    Mapping(std::string name,
            setptr_t domain = setptr_t(new UniversalSet()),
            setptr_t range = setptr_t(new UniversalSet())) : name(name), Function(domain, range) {}

    virtual Object *copyThis() { return new Mapping(*this); }

    virtual std::string toString() { return this->name + ":" + this->domain->toString() + "->" + this->range->toString(); }

    virtual objptr_t operator()(funcargs_t);

    std::string name;
};