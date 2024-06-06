#pragma once

#include "Set.h"

/*有理数集*/
class RationalSet : public Set
{
public:
    virtual Object *copyThis() { return new RationalSet(); }

    virtual boolptr_t contains(objptr_t);

    virtual std::string toString() { return "Q"; }
};