#pragma once

#include <stdexcept>
#include <iostream>

#include "Object.h"
#include "Set.h"

typedef std::vector<objptr_t> funcargs_t;

class Mapping;
/*函数*/
class Function : virtual public Object
{
public:
    Function(std::string name, funcargs_t args) : name(name), args(args), mapping(nullptr) {}
    Function(std::shared_ptr<Mapping>, funcargs_t);

    virtual std::string toString()
    {
        std::string result = this->name + "(";
        for (size_t i = 0; i < args.size(); i++)
        {
            result += args[i]->toString();
            if (i != args.size() - 1)
                result += ", ";
        }
        result += ")";
        return result;
    }

    virtual boolptr_t operator==(objptr_t);

    virtual Object *copyThis() { return new Function(*this); }

    virtual objptr_t replace(objptr_t, objptr_t);

    virtual void setArgs(funcargs_t args) { this->args = args; }
    virtual objptr_t _simplify_() { return objptr_t(copyThis()); }

    std::string name;
    funcargs_t args;
    std::shared_ptr<Mapping> mapping;
};

typedef std::shared_ptr<Function> funcptr_t;

boolptr_t in_domain(setptr_t, funcargs_t);