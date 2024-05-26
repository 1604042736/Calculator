#pragma once

#include "Set.h"

/*整数集*/
class IntegerSet : public Set
{
public:
    virtual Object *copyThis() { return new IntegerSet(); }

    virtual std::string toString() { return "Z"; }
};