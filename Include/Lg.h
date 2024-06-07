#pragma once

#include "Log.h"
#include "Integer.h"

class Lg : public Log
{
public:
    Lg(exprptr_t tnum) : Log(exprptr_t(new Integer(10)), tnum) {}

    virtual Object *copyThis() { return new Lg(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Lg(*this)); }

    virtual std::string toString() { return "lg(" + this->tnum->toString() + ")"; }
};

class LgMapping : public ExprMapping
{
public:
    LgMapping() : ExprMapping("lg", setptr_t(new RealSet())) {}
    virtual objptr_t operator()(funcargs_t);
};