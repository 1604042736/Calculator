#pragma once

#include <limits>

#include "Number.h"
#include "True.h"
#include "False.h"
#include "Integer.h"

/*无穷大*/
class Infinity : public Number
{
public:
    Infinity() : sign(SIGN_POSITIVE) {}
    Infinity(Sign sign) : sign(sign) {}
    Infinity(int sign) { this->sign = sign >= 0 ? SIGN_POSITIVE : SIGN_NEGATIVE; }

    virtual Object *copyThis() { return new Infinity(*this); }

    virtual std::string toString() { return sign == SIGN_POSITIVE ? "oo" : "-oo"; }

    virtual double toDouble() { return std::numeric_limits<double>::infinity() * this->sign; }

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t reciprocal();
    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t abs() { return exprptr_t(new Infinity()); }
    virtual exprptr_t opposite() { return exprptr_t(new Infinity(-sign)); }

    virtual boolptr_t operator>(exprptr_t) { return boolptr_t(new True()); }
    virtual boolptr_t operator<(exprptr_t) { return boolptr_t(new False()); }
    virtual boolptr_t operator==(exprptr_t);

    Sign sign;
};