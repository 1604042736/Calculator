#pragma once

#include "Boolean.h"

class False : public Boolean
{
public:
    virtual Object *copyThis() { return new False(*this); }

    virtual boolptr_t operator&&(boolptr_t);
    virtual boolptr_t operator!();

    virtual bool operator==(boolptr_t);

    virtual std::string toString() { return "False"; }
};