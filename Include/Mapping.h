#pragma once

#include "Function.h"
#include "Expression.h"

/*映射*/
class Mapping : public Function
{
public:
    Mapping(std::string name,
            setptr_t domain = setptr_t(new UniversalSet()),
            setptr_t range = setptr_t(new UniversalSet())) : name(name), Function(domain, range) {}

    virtual Object *copyThis() { return new Mapping(*this); }

    virtual std::string toString() { return this->name + ":" + this->domain->toString() + "->" + this->range->toString(); }

    virtual objptr_t operator()(funcargs_t);

    std::string name;
};

template <class Base, class ArgBaseT, class FuncT, class SymbolT>
class SArgFuncMapping : public Base
{
    static_assert(std::is_base_of<Mapping, Base>::value, "SArgFuncMapping的基类必须是Mapping及其派生类");
    typedef std::shared_ptr<ArgBaseT> ArgT;

public:
    SArgFuncMapping() : Base("???")
    {
        ArgT arg(new SymbolT("x"));
        FuncT func(arg);
        this->name = func.name;
        this->domain = func.domain;
        this->range = func.range;
    }

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1)
            throw std::runtime_error("[SArgFuncMapping(" + this->name + ")::operator()]超出定义域");
        ArgBaseT *t = dynamic_cast<ArgBaseT *>(args[0].get());
        if (t == nullptr)
            throw std::runtime_error("[SArgFuncMapping(" + this->name + ")::operator()]超出定义域");
        ArgT ret(new FuncT(ArgT(dynamic_cast<ArgBaseT *>(t->copyThis()))));
        if (std::is_base_of<Expression, ArgBaseT>::value)
            ret = ret->simplify();
        return ret;
    }
};