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

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t reciprocal();
    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t abs() { return exprptr_t(new Infinity()); }

    virtual boolptr_t operator>(exprptr_t) { return boolptr_t(new False()); }
    virtual boolptr_t operator<(exprptr_t) { return boolptr_t(new True()); }
    virtual boolptr_t operator==(exprptr_t);
};