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
class SArgFunction_T : virtual public Base
{
    static_assert(std::is_base_of<Function, Base>::value, "SArgFunction的基类必须是Function及其派生类");
    typedef std::shared_ptr<ArgBaseT> ArgT;

public:
    SArgFunction_T(std::string name,
                   ArgT arg,
                   setptr_t domain = setptr_t(new UniversalSet()),
                   setptr_t range = setptr_t(new UniversalSet()))
        : name(name), arg(arg), Base(domain, range)
    {
        this->domain = domain;
        this->range = range;
    }

    virtual std::string toString() { return name + "(" + arg->toString() + ")"; }

    virtual boolptr_t operator==(ArgT b)
    {
        SArgFunction_T *t = dynamic_cast<SArgFunction_T *>(b.get());
        if (t != nullptr)
            return to_boolean(name == t->name) && arg == t->arg;
        return Base::operator==(b);
    }

    virtual objptr_t replace(objptr_t old, objptr_t _new)
    {
        objptr_t arg = this->arg->replace(old, _new);
        ArgBaseT *t = dynamic_cast<ArgBaseT *>(arg.get());
        if (t == nullptr)
            throw std::runtime_error("[SArgFunction_T(" + this->name + ")::replace]超出定义域");
        SArgFunction_T *sf = dynamic_cast<SArgFunction_T *>(this->copyThis());
        sf->arg = ArgT(dynamic_cast<ArgBaseT *>(arg->copyThis()));
        ArgT ret = ArgT(dynamic_cast<ArgBaseT *>(sf->copyThis()));
        delete sf;
        return ret;
    }

    std::string name;
    ArgT arg;
};

template <class Base, class ArgBaseT>
/*多参函数*/
class MArgFunction_T : virtual public Base
{
    typedef std::shared_ptr<ArgBaseT> ArgT;
    typedef std::vector<ArgT> ArgsT;

public:
    MArgFunction_T(std::string name,
                   ArgsT args,
                   setptr_t domain = setptr_t(new UniversalSet()),
                   setptr_t range = setptr_t(new UniversalSet()))
        : name(name), args(args), Base(domain, range)
    {
        this->domain = domain;
        this->range = range;
    }

    virtual std::string toString()
    {
        std::string result = name + "(" + args[0]->toString();
        for (size_t i = 1; i < args.size(); i++)
            result = result + "," + args[i]->toString();
        result += ")";
        return result;
    }

    virtual boolptr_t operator==(ArgT b)
    {
        MArgFunction_T *t = dynamic_cast<MArgFunction_T *>(b.get());
        if (t != nullptr)
        {
            if (this->args.size() != t->args.size())
                return to_boolean(false);
            boolptr_t result = to_boolean(name == t->name);
            for (size_t i = 0; i < this->args.size(); i++)
                result = result->operator&&(this->args[i]->operator==(t->args[i]));
            return result;
        }
        return Base::operator==(b);
    }

    virtual objptr_t replace(objptr_t old, objptr_t _new)
    {
        ArgsT args;
        for (size_t i = 0; i < this->args.size(); i++)
        {
            objptr_t arg = this->args[i]->replace(old, _new);
            ArgBaseT *t = dynamic_cast<ArgBaseT *>(arg.get());
            if (t == nullptr)
                throw std::runtime_error("[MArgFunction_T(" + this->name + ")::replace]超出定义域");
            args.push_back(ArgT(dynamic_cast<ArgBaseT *>(arg->copyThis())));
        }
        MArgFunction_T *mf = dynamic_cast<MArgFunction_T *>(this->copyThis());
        mf->args = args;
        ArgT ret = ArgT(dynamic_cast<ArgBaseT *>(mf->copyThis()));
        delete mf;
        return ret;
    }

    std::string name;
    ArgsT args;
};

typedef std::shared_ptr<Function> funcptr_t;
typedef SArgFunction_T<Function, Object> SArgFunction;
typedef MArgFunction_T<Function, Object> MArgFunction;