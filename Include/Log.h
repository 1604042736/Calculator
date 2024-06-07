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

    virtual objptr_t replace(objptr_t, objptr_t);

    virtual exprptr_t _simplify();

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