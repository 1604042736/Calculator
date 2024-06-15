#pragma once

#include "Mapping.h"

class SetMapping : public Mapping
{
public:
    SetMapping(std::string name,
               mappingsec_t sections = {},
               setptr_t domain = setptr_t(new UniversalSet()),
               setptr_t range = setptr_t(new UniversalSet())) : Mapping(name, sections, domain, range) {}

    virtual Object *copyThis() { return new SetMapping(*this); }

    virtual objptr_t operator()(funcargs_t);
};