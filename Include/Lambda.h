#pragma once

#include "Function.h"
#include "Symbol.h"

// 匿名函数的参数一定是某种类型的符号
typedef std::vector<symptr_t> lambdaargs_t;
typedef objptr_t funcbodyptr_t;

/*匿名函数*/
class Lambda : public Function
{
public:
    Lambda(lambdaargs_t, funcbodyptr_t,
           setptr_t domain = setptr_t(new UniversalSet()),
           setptr_t range = setptr_t(new UniversalSet()));

    virtual Object *copyThis() { return new Lambda(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual objptr_t operator()(funcargs_t);

    funcbodyptr_t body; // 函数表达式, 注意和bodyession(代数表达式)区分
    lambdaargs_t args;
};