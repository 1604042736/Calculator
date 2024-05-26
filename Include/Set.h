#pragma once

#include "Object.h"
#include "Boolean.h"
#include "False.h"

class Set;
typedef std::shared_ptr<Set> setptr_t;

class EmptySet;
/*集合*/
class Set : public Object
{
public:
    virtual setptr_t operator&(setptr_t);
    virtual setptr_t operator&(objptr_t);
    virtual setptr_t operator|(setptr_t);
    virtual setptr_t operator|(objptr_t);

    virtual setptr_t inline copyToSetPtr() { return setptr_t(dynamic_cast<Set *>(this->copyThis())); }

    virtual boolptr_t contains(objptr_t);
    virtual boolptr_t contains(Object &&);
    virtual boolptr_t includes(setptr_t);
};