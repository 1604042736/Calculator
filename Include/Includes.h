#pragma once

#include "Boolean.h"
#include "Set.h"

/*包含*/
class Includes : public Boolean
{
public:
    Includes(setptr_t subset, setptr_t set) : subset(subset), set(set) {}

    virtual Object *copyThis() { return new Includes(*this); }

    virtual std::string toString() { return this->subset->toString() + " in " + this->set->toString(); }

    setptr_t subset;
    setptr_t set;
};