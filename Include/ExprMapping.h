#pragma once

#include "Mapping.h"
#include "Expression.h"

class ExprMapping : public Mapping, public Expression
{
public:
    ExprMapping(std::string name,
                setptr_t domain = setptr_t(new UniversalSet()),
                setptr_t range = setptr_t(new RealSet())) : Mapping(name, domain, range) {}

    virtual Object *copyThis() { return new ExprMapping(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new ExprMapping(*this)); }

    virtual objptr_t operator()(funcargs_t);
};