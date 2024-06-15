#pragma once

#include "Mapping.h"
#include "RealSet.h"
#include "UniversalSet.h"

class ExprMapping : public Mapping
{
public:
    ExprMapping(std::string name,
                mappingsec_t sections = {},
                setptr_t domain = setptr_t(new UniversalSet()),
                setptr_t range = setptr_t(new RealSet())) : Mapping(name, sections, domain, range) {}

    virtual Object *copyThis() { return new ExprMapping(*this); }

    virtual objptr_t operator()(funcargs_t);
};