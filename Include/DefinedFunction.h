#pragma once

#include "Function.h"
#include "Lambda.h"

typedef std::vector<Lambda> deffuncsec_t;

/*定义了的函数*/
class DefinedFunction : virtual public Function
{
public:
    DefinedFunction(std::string name,
                    funcargs_t args,
                    deffuncsec_t *sections,
                    setptr_t domain = setptr_t(new UniversalSet()),
                    setptr_t range = setptr_t(new UniversalSet())) : name(name), args(args), sections(sections), Function(domain, range) {}
    virtual Object *copyThis() { return new DefinedFunction(*this); }

    virtual std::string toString();

    virtual boolptr_t operator==(objptr_t);
    virtual objptr_t operator()(funcargs_t);

    virtual objptr_t replace(objptr_t, objptr_t);

    std::string name;
    deffuncsec_t *sections; // 函数每段的表达式
    funcargs_t args;
};