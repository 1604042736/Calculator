#pragma once

#include "ExprFunction.h"
#include "ExprMapping.h"

class Log : public ExprFunction
{
public:
    Log(exprptr_t, exprptr_t);

    virtual Object *copyThis() { return new Log(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new Log(*this)); }

    virtual std::string toString();

    virtual exprptr_t diff(exprptr_t);
    virtual boolptr_t operator==(exprptr_t);

    virtual exprptr_t _simplify();

    virtual void setArgs(funcargs_t args)
    {
        this->base = dynamic_cast<Expression *>(args[0].get())->copyToExprPtr();
        this->tnum = dynamic_cast<Expression *>(args[1].get())->copyToExprPtr();
        ExprFunction::setArgs(args);
    }

    exprptr_t base; // 底数
    exprptr_t tnum; // 真数
};

class LogMapping : public ExprMapping
{
public:
    LogMapping();
    virtual exprptr_t operator()(exprptr_t, exprptr_t);
    virtual objptr_t operator()(funcargs_t);
};