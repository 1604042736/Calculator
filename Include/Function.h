#pragma once

#include <stdexcept>
#include <iostream>

#include "Object.h"
#include "Set.h"
#include "UniversalSet.h"

typedef std::vector<objptr_t> funcargs_t;

/*函数*/
class Function : virtual public Object
{
public:
    Function(setptr_t domain = setptr_t(new UniversalSet()),
             setptr_t range = setptr_t(new UniversalSet()));

    virtual boolptr_t inDomain(funcargs_t);

    /*定义域和值域指的是外层函数的定义域和值域, 并且两者不一定同步*/
    setptr_t domain; // 定义域
    setptr_t range;  // 值域
};

template <class Base, class ArgBaseT>
/*单参函数*/
class SArgFunction : virtual public Base
{
    static_assert(std::is_base_of<Function, Base>::value, "SArgFunction的基类必须是Function及其派生类");
    typedef std::shared_ptr<ArgBaseT> ArgT;

public:
    SArgFunction(std::string name,
                 ArgT arg,
                 setptr_t domain = setptr_t(new UniversalSet()),
                 setptr_t range = setptr_t(new UniversalSet()))
        : name(name), arg(arg), Base(domain, range) {}

    virtual std::string toString() { return name + "(" + arg->toString() + ")"; }

    virtual boolptr_t operator==(ArgT b)
    {
        SArgFunction *t = dynamic_cast<SArgFunction *>(b.get());
        if (t != nullptr)
            return to_boolean(name == t->name) && arg == t->arg;
        return Base::operator==(b);
    }

    virtual objptr_t replace(objptr_t old, objptr_t _new)
    {
        objptr_t arg = this->arg->replace(old, _new);
        ArgBaseT *t = dynamic_cast<ArgBaseT *>(arg.get());
        if (t == nullptr)
            throw std::runtime_error("[SArgFunction(" + this->name + ")::replace]超出定义域");
        SArgFunction *sf = dynamic_cast<SArgFunction *>(this->copyThis());
        sf->arg = ArgT(dynamic_cast<ArgBaseT *>(arg->copyThis()));
        ArgT ret = ArgT(dynamic_cast<ArgBaseT *>(sf->copyThis()));
        delete sf;
        return ret;
    }

    std::string name;
    ArgT arg;
};

typedef std::shared_ptr<Function> funcptr_t;