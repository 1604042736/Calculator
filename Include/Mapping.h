#pragma once

#include "Function.h"
#include "Lambda.h"
#include "UniversalSet.h"

typedef std::vector<Lambda> mappingsec_t;

/*映射*/
class Mapping : public Function, public std::enable_shared_from_this<Mapping>
{
public:
    Mapping(std::string name,
            mappingsec_t sections = {},
            setptr_t domain = setptr_t(new UniversalSet()),
            setptr_t range = setptr_t(new UniversalSet()))
        : sections(sections), domain(domain), range(range), Function(name, {}) {}

    virtual Object *copyThis() { return new Mapping(*this); }

    virtual std::string toString() { return this->name + ":" + this->domain->toString() + "->" + this->range->toString(); }

    virtual objptr_t operator()(funcargs_t);

    setptr_t domain, range;
    mappingsec_t sections;
};

typedef std::shared_ptr<Mapping> mappingptr_t;