#pragma once

#include "Set.h"
#include "IntegerSet.h"
#include "RationalSet.h"
#include "True.h"

/*实数集*/
class RealSet : public Set
{
public:
    virtual Object *copyThis() { return new RealSet(); }

    virtual std::string toString() { return "R"; }

    virtual boolptr_t includes(setptr_t);
};