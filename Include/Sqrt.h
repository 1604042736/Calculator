#pragma once

#include "Pow.h"
#include "ExprMapping.h"
#include "Interval.h"
#include "Integer.h"
#include "Infinity.h"

/*
开方运算
作为幂运算的一种特例
*/
class Sqrt : public Pow
{
public:
    Sqrt(exprptr_t);
    Sqrt(Expression &arg) : Sqrt(exprptr_t(dynamic_cast<Expression *>(arg.copyThis()))) {}

    virtual Object *copyThis() { return new Sqrt(*this); }

    virtual inline bool isCommutative() { return false; }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();
    virtual std::string toLateX();

    virtual exprptr_t _simplify();
    virtual exprptr_t _eval(Integer);

    exprptr_t arg;
};

class SqrtMapping : public ExprMapping
{
public:
    SqrtMapping() : ExprMapping("sqrt",
                                {},
                                setptr_t(new Interval(exprptr_t(new Integer(0)),
                                                      exprptr_t(new Infinity()), false, true)),
                                setptr_t(new Interval(exprptr_t(new Integer(0)),
                                                      exprptr_t(new Infinity()), false, true))) {}
    virtual exprptr_t operator()(exprptr_t);
    virtual objptr_t operator()(funcargs_t);
};