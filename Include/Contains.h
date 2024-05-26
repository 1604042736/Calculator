#pragma once

#include "Boolean.h"
#include "Set.h"

/*包含运算符*/
class Contains : public Boolean
{
public:
    Contains(objptr_t element, setptr_t set) : element(element), set(set) {}

    virtual Object *copyThis() { return new Contains(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    objptr_t element;
    setptr_t set;
};