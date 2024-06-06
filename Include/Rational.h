#pragma once

#include <string>

#include "Object.h"
#include "Integer.h"
#include "Float.h"
#include "Expression.h"
#include "RationalSet.h"
#include "Number.h"

/*有理数*/
class Rational : public Number
{
public:
    Rational(Integer, Integer);
    Rational(Float, Float);
    Rational(int32_t, int32_t);
    Rational(Integer);
    Rational(Float);
    Rational(double);
    Rational(std::string);

    virtual std::string toString();
    virtual prettystring_t toPrettyString();
    virtual std::string toLateX();

    virtual Object *copyThis() { return new Rational(*this); }

    virtual Rational operator+(Rational);
    virtual Rational operator-(Rational);
    virtual Rational operator*(Rational);
    virtual Rational operator/(Rational);

    friend Rational operator+(Integer a, Rational b) { return Rational(a) + b; }
    friend Rational operator-(Integer a, Rational b) { return Rational(a) - b; }
    friend Rational operator*(Integer a, Rational b) { return Rational(a) * b; }
    friend Rational operator/(Integer a, Rational b) { return Rational(a) / b; }

    friend Rational operator+(Float a, Rational b) { return Rational(a) + b; }
    friend Rational operator-(Float a, Rational b) { return Rational(a) - b; }
    friend Rational operator*(Float a, Rational b) { return Rational(a) * b; }
    friend Rational operator/(Float a, Rational b) { return Rational(a) / b; }

    virtual bool operator>(Rational b) { return this->nume * b.deno > this->deno * b.nume; }
    virtual bool operator>=(Rational b) { return (*this > b) || (*this == b); }
    virtual bool operator<(Rational b) { return this->nume * b.deno < this->deno * b.nume; }
    virtual bool operator<=(Rational b) { return (*this < b) || (*this == b); }
    virtual bool operator==(Rational b) { return this->nume == b.nume && this->deno == b.deno; }
    virtual bool operator!=(Rational b) { return !(*this == b); }

    friend bool operator>(Integer a, Rational b) { return Rational(a) > b; }
    friend bool operator>=(Integer a, Rational b) { return Rational(a) >= b; }
    friend bool operator<(Integer a, Rational b) { return Rational(a) < b; }
    friend bool operator<=(Integer a, Rational b) { return Rational(a) <= b; }
    friend bool operator==(Integer a, Rational b) { return Rational(a) == b; }
    friend bool operator!=(Integer a, Rational b) { return Rational(a) != b; }

    friend bool operator>(Float a, Rational b) { return Rational(a) > b; }
    friend bool operator>=(Float a, Rational b) { return Rational(a) >= b; }
    friend bool operator<(Float a, Rational b) { return Rational(a) < b; }
    friend bool operator<=(Float a, Rational b) { return Rational(a) <= b; }
    friend bool operator==(Float a, Rational b) { return Rational(a) == b; }
    friend bool operator!=(Float a, Rational b) { return Rational(a) != b; }

    virtual Rational pow(Integer);

    virtual Float pow(Integer, Integer);

    virtual exprptr_t reciprocal() { return exprptr_t(new Rational(this->deno, this->nume)); }
    virtual exprptr_t opposite() { return exprptr_t(new Rational(*(Integer *)this->nume.opposite().get(), this->deno)); }

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator-(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t operator/(exprptr_t);
    virtual boolptr_t operator>(exprptr_t);
    virtual boolptr_t operator<(exprptr_t);
    virtual boolptr_t operator==(exprptr_t);
    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t pow(exprptr_t, Integer);
    virtual exprptr_t abs() { return exprptr_t(new Rational(Integer(this->nume.sections, SIGN_POSITIVE), Integer(this->deno.sections, SIGN_POSITIVE))); }

    virtual exprptr_t _simplify();
    virtual exprptr_t _eval(Integer);

    void reduction();

    Integer nume; // 分子
    Integer deno; // 分母
};