#pragma once

#include "ExprFunction.h"
#include "ExprMapping.h"
#include "Interval.h"
#include "Integer.h"
#include "Infinity.h"

/*绝对值函数*/
class Abs : public ExprFunction
{
public:
    Abs(exprptr_t);

    virtual Object *copyThis() { return new Abs(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Abs(*this)); }

    virtual exprptr_t abs() { return this->copyToExprPtr(); }
    virtual exprptr_t diff(exprptr_t);

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual void setArgs(funcargs_t args)
    {
        this->arg = dynamic_cast<Expression *>(args[0].get())->copyToExprPtr();
        ExprFunction::setArgs(args);
    }

    exprptr_t arg;
};

class AbsMapping : public ExprMapping
{
public:
    AbsMapping() : ExprMapping("abs",
                               {},
                               setptr_t(new RealSet()),
                               setptr_t(new Interval(exprptr_t(new Integer(0)),
                                                     exprptr_t(new Infinity()), false, true))) {}
    virtual exprptr_t operator()(exprptr_t);
    virtual objptr_t operator()(funcargs_t);
};