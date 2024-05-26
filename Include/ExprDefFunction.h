#pragma once

#include "ExprFunction.h"
#include "DefinedFunction.h"

class ExprDefFunction : public Expression, public DefinedFunction
{
public:
    ExprDefFunction(std::string name,
                    funcargs_t args,
                    deffuncsec_t *sections,
                    setptr_t domain = setptr_t(new UniversalSet()),
                    setptr_t range = setptr_t(new RealSet())) : DefinedFunction(name, args, sections, domain, range) {}

    virtual Object *copyThis() { return new ExprDefFunction(*this); }

    virtual boolptr_t operator==(objptr_t b) { return DefinedFunction::operator==(b); }

    virtual exprptr_t _simplify();
};