#pragma once

#include "Set.h"

/*全集, 包含所有Object*/
class UniversalSet : public Set
{
public:
    virtual Object *copyThis() { return new UniversalSet(); }

    virtual std::string toString() { return "U"; }

    virtual setptr_t operator&(setptr_t b) { return b; }
    virtual setptr_t operator|(setptr_t) { return setptr_t(new UniversalSet()); }

    virtual boolptr_t contains(objptr_t);
    virtual boolptr_t includes(setptr_t);
};