#pragma once

#include "Object.h"
#include "Expression.h"
#include "Number.h"
#include "Mul.h"
#include "Integer.h"
#include "Pow.h"
#include "Interval.h"

/*自然常数*/
class E : public Number
{
public:
    virtual std::string toString() { return "e"; }

    virtual bool operator==(E) { return true; }

    virtual Mul operator+(E) { return Mul({this->copyToExprPtr(), exprptr_t(new Integer(2))}); }
    virtual Integer operator-(E) { return 0; }
    virtual Pow operator*(E) { return Pow({this->copyToExprPtr(), exprptr_t(new Integer(2))}); }
    virtual Integer operator/(E) { return 1; }

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator-(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t operator/(exprptr_t);

    virtual boolptr_t operator==(exprptr_t);

    virtual exprptr_t eval(Integer);

    virtual Object *copyThis() { return new E(*this); }

    virtual setptr_t belongto() { return setptr_t(new Interval(exprptr_t(new Integer(2)), exprptr_t(new Integer(3)), true, true)); }
};