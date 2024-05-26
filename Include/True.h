#pragma once

#include "Boolean.h"

class True : public Boolean
{
public:
    virtual Object *copyThis() { return new True(*this); }

    virtual boolptr_t operator||(boolptr_t);
    virtual boolptr_t operator!();

    virtual std::string toString() { return "True"; }
};