#pragma once

#include <string>

#include "Object.h"
#include "Integer.h"
#include "Expression.h"
#include "RationalSet.h"
#include "Number.h"

class Rational;
/*浮点数*/
class Float : public Number
{
public:
    Float(Integer, Integer);
    Float(std::string);
    Float(Integer);
    Float(double);
    Float(uint64_t);
    Float(int64_t);
    Float(int32_t);
    Float(Rational, Integer);

    virtual std::string toString();

    virtual Object *copyThis() { return new Float(*this); }

    virtual Float operator+(Float);
    virtual Float operator-(Float);
    virtual Float operator*(Float);
    virtual Rational operator/(Float);

    friend Float operator+(Integer a, Float b) { return Float(a) + b; }
    friend Float operator-(Integer a, Float b) { return Float(a) - b; }
    friend Float operator*(Integer a, Float b) { return Float(a) * b; }
    friend Rational operator/(Integer a, Float b);

    virtual bool operator>(Float b) { return this->exponent > b.exponent || this->significand > b.significand; }
    virtual bool operator>=(Float b) { return (*this > b) || (*this == b); }
    virtual bool operator<(Float b) { return this->exponent < b.exponent || this->significand < b.significand; }
    virtual bool operator<=(Float b) { return (*this < b) || (*this == b); }
    virtual bool operator==(Float b) { return this->exponent == b.exponent && this->significand == b.significand; }
    virtual bool operator!=(Float b) { return !(*this == b); }

    virtual bool operator>(int b) { return *this > Float(b); }
    virtual bool operator>=(int b) { return *this >= Float(b); }
    virtual bool operator<(int b) { return *this < Float(b); }
    virtual bool operator<=(int b) { return *this <= Float(b); }
    virtual bool operator==(int b) { return *this == Float(b); }
    virtual bool operator!=(int b) { return *this != Float(b); }

    friend bool operator>(Integer a, Float b) { return Float(a) > b; }
    friend bool operator>=(Integer a, Float b) { return Float(a) >= b; }
    friend bool operator<(Integer a, Float b) { return Float(a) < b; }
    friend bool operator<=(Integer a, Float b) { return Float(a) <= b; }
    friend bool operator==(Integer a, Float b) { return Float(a) == b; }
    friend bool operator!=(Integer a, Float b) { return Float(a) != b; }

    virtual exprptr_t pow(Integer);

    virtual Float pow(Float, Integer);
    virtual Float pow(Integer, Integer);

    virtual exprptr_t reciprocal();
    virtual exprptr_t opposite() { return exprptr_t(new Float(*(Integer *)this->significand.opposite().get(), this->exponent)); }

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator-(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t operator/(exprptr_t);
    virtual boolptr_t operator>(exprptr_t);
    virtual boolptr_t operator<(exprptr_t);
    virtual boolptr_t operator==(exprptr_t);
    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t pow(exprptr_t, Integer);
    virtual exprptr_t abs() { return exprptr_t(new Float(Integer(this->significand.sections, SIGN_POSITIVE), this->exponent)); }

    virtual exprptr_t eval(Integer);

    Integer significand; // 尾数(有效数), 同时决定了Float的符号
    Integer exponent;    // 指数
};

std::vector<Integer> exponent_matching(Float, Float);