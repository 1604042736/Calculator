#pragma once

#include "Pow.h"
#include "Integer.h"

/*倒数*/
class Reciprocal : public Pow
{
public:
    Reciprocal(exprptr_t expr) : Pow({expr, exprptr_t(new Integer(-1))}) {}

    virtual Object *copyThis() { return new Reciprocal(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();
    virtual std::string toLateX();

    virtual exprptr_t reciprocal();

    exprptr_t _simplify();
    virtual exprptr_t _eval(Integer);
};