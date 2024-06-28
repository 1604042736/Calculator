#pragma once

#include "Set.h"
#include "SetSymbol.h"

/*空集*/
class EmptySet : public SetSymbol
{
public:
    EmptySet() : SetSymbol("∅") {}
    virtual Object *copyThis() { return new EmptySet(); }

    virtual setptr_t operator&(setptr_t) { return setptr_t(new EmptySet()); }
    virtual setptr_t operator|(setptr_t b) { return b; }

    virtual boolptr_t contains(objptr_t);
    virtual boolptr_t includes(setptr_t);

    virtual boolptr_t operator==(setptr_t);
};