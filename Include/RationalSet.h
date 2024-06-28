#pragma once

#include "Set.h"
#include "SetSymbol.h"

/*有理数集*/
class RationalSet : public SetSymbol
{
public:
    RationalSet() : SetSymbol("Q") {}
    virtual Object *copyThis() { return new RationalSet(); }

    virtual boolptr_t contains(objptr_t);
};