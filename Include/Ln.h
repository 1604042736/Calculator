#pragma once

#include "Log.h"
#include "E.h"

class Ln : public Log
{
public:
    Ln(exprptr_t tnum) : Log(exprptr_t(new E()), tnum) {}

    virtual Object *copyThis() { return new Ln(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Ln(*this)); }

    virtual std::string toString() { return "ln(" + this->tnum->toString() + ")"; }
};

class LnMapping : public ExprMapping
{
public:
    LnMapping() : ExprMapping("ln", setptr_t(new RealSet())) {}
    virtual objptr_t operator()(funcargs_t);
};