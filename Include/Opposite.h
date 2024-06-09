#pragma once

#include "Mul.h"
#include "Integer.h"

/*相反数*/
class Opposite : public Mul
{
public:
    Opposite(exprptr_t expr) : Mul({exprptr_t(new Integer(-1)), expr}) {}

    virtual Object *copyThis() { return new Opposite(*this); }

    virtual exprptr_t abs() { return this->opposite()->abs(); }

    virtual boolptr_t operator>(exprptr_t b) { return !(this->opposite() > b); }
    virtual boolptr_t operator<(exprptr_t b) { return !(this->opposite() < b); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual exprptr_t _simplify();
    virtual exprptr_t _eval(Integer);
};