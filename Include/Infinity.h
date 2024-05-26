#pragma once

#include "Number.h"
#include "True.h"
#include "False.h"

/*无穷大*/
class Infinity : public Number
{
public:
    virtual Object *copyThis() { return new Infinity(); }

    virtual std::string toString() { return "oo"; }

    virtual boolptr_t operator>(exprptr_t) { return boolptr_t(new False()); }
    virtual boolptr_t operator<(exprptr_t) { return boolptr_t(new True()); }
};