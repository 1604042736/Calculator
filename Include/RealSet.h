#pragma once

#include "Set.h"
#include "IntegerSet.h"
#include "RationalSet.h"
#include "True.h"
#include "SetSymbol.h"

/*实数集*/
class RealSet : public SetSymbol
{
public:
    RealSet() : SetSymbol("R") {}

    virtual Object *copyThis() { return new RealSet(); }

    virtual boolptr_t contains(objptr_t);
    virtual boolptr_t includes(setptr_t);
};