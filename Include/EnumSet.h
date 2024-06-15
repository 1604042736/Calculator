#pragma once

#include "Set.h"

typedef std::vector<objptr_t> elements_t;

/*以枚举方式表示的集合*/
class EnumSet : public Set
{
public:
    /*创建空集*/
    EnumSet() {}
    EnumSet(elements_t elements) : elements(elements) {}

    virtual Object *copyThis() { return new EnumSet(*this); }

    virtual EnumSet operator|(EnumSet);
    virtual setptr_t operator|(setptr_t);

    virtual setptr_t operator&(EnumSet);
    virtual setptr_t operator&(setptr_t);

    virtual setptr_t product(EnumSet);
    virtual setptr_t product(setptr_t);
    virtual setptr_t productpow(Integer);

    virtual boolptr_t contains(objptr_t);

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual boolptr_t operator==(EnumSet *);
    virtual boolptr_t operator==(setptr_t);

    virtual setptr_t _simplify();

    virtual objptr_t replace(objptr_t, objptr_t);

    elements_t elements; // 集合元素
};