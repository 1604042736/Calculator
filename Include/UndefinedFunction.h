#pragma once

#include "Function.h"

/*未定义的函数*/
class UndefinedFunction : public Function
{
public:
    UndefinedFunction(std::string, funcargs_t,
                      setptr_t domain = setptr_t(new UniversalSet()),
                      setptr_t range = setptr_t(new UniversalSet()));

    virtual Object *copyThis() { return new UndefinedFunction(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    std::string name;
    funcargs_t args;
};