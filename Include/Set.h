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
    virtual setptr_t operator&(objptr_t);
    virtual setptr_t operator|(setptr_t);
    virtual setptr_t operator|(objptr_t);

    virtual setptr_t operator*(setptr_t);
    virtual setptr_t operator*(Set &b) { return this->operator*(b.copyToSetPtr()); }
    virtual setptr_t operator*(Set &&b) { return this->operator*(b.copyToSetPtr()); }
    friend setptr_t operator*(setptr_t a, setptr_t b) { return a->operator*(b); }
    virtual setptr_t pow(Integer);

    /*这里的运算指的是集合中的元素参与运算后的结果所在的集合*/
    virtual setptr_t operator+(setptr_t);
    virtual setptr_t mul(setptr_t);
    virtual setptr_t pow(setptr_t);

    virtual setptr_t inline copyToSetPtr() { return setptr_t(dynamic_cast<Set *>(this->copyThis())); }

    virtual boolptr_t contains(objptr_t);
    virtual boolptr_t contains(Object &&);
    virtual boolptr_t includes(setptr_t);
};