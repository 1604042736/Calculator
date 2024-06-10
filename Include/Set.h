#pragma once

#include "Object.h"
#include "Boolean.h"
#include "False.h"

class Set;
typedef std::shared_ptr<Set> setptr_t;
class Integer;

class EmptySet;
/*集合*/
class Set : public Object
{
public:
    virtual setptr_t operator&(setptr_t);
    virtual objptr_t operator&(objptr_t);
    virtual setptr_t operator|(setptr_t);
    virtual objptr_t operator|(objptr_t);

    virtual setptr_t product(setptr_t);
    virtual setptr_t product(Set &b) { return this->product(b.copyToSetPtr()); }
    virtual setptr_t product(Set &&b) { return this->product(b.copyToSetPtr()); }
    friend setptr_t product(setptr_t a, setptr_t b) { return a->product(b); }
    virtual setptr_t productpow(Integer);

    virtual boolptr_t operator==(setptr_t b) { return to_boolean(this == b.get()); }
    virtual boolptr_t operator==(objptr_t);

    /*这里的运算指的是集合中的元素参与运算后的结果所在的集合*/
    virtual setptr_t add(setptr_t);
    virtual setptr_t mul(setptr_t);
    virtual setptr_t pow(setptr_t);

    virtual objptr_t pow(objptr_t);

    virtual setptr_t inline copyToSetPtr() { return setptr_t(dynamic_cast<Set *>(this->copyThis())); }

    virtual boolptr_t contains(objptr_t);
    virtual boolptr_t contains(Object &&);
    virtual boolptr_t includes(setptr_t);

    /*化简*/
    virtual setptr_t simplify();
    /*化简一次*/
    virtual setptr_t _simplify()
    {
        return this->copyToSetPtr();
    }
};