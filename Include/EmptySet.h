#pragma once

#include "Set.h"

/*空集*/
class EmptySet : public Set
{
public:
    virtual Object *copyThis() { return new EmptySet(); }

    virtual std::string toString() { return "∅"; }

    virtual setptr_t operator&(setptr_t) { return setptr_t(new EmptySet()); }
    virtual setptr_t operator|(setptr_t b) { return b; }

    virtual boolptr_t contains(objptr_t);
    virtual boolptr_t includes(setptr_t);
};