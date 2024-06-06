#pragma once

#include "Set.h"

/*整数集*/
class IntegerSet : public Set
{
public:
    virtual Object *copyThis() { return new IntegerSet(); }

    virtual boolptr_t contains(objptr_t);

    virtual std::string toString() { return "Z"; }
};