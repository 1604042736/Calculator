#pragma once

#include "Set.h"
#include "SetSymbol.h"

/*整数集*/
class IntegerSet : public SetSymbol
{
public:
    IntegerSet() : SetSymbol("Z") {}
    virtual Object *copyThis() { return new IntegerSet(); }

    virtual boolptr_t contains(objptr_t);
};