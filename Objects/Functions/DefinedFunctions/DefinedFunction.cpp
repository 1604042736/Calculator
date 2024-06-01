#include <stdexcept>

#include "DefinedFunction.h"
#include "ExprDefFunction.h"
#include "Tuple.h"
#include "Common.h"

std::string DefinedFunction::toString()
{
    std::string result = this->name + "(" + this->args[0]->toString();
    for (size_t i = 1; i < this->args.size(); i++)
        result += " , " + this->args[i]->toString();
    result += ")";
    return result;
    // 防止递归调用
    result += "[" + (*this->sections)[0].toString() + "]";
    for (size_t i = 1; i < this->sections->size(); i++)
        result += " , [" + (*this->sections)[i].toString() + "]";
    return result;
}

boolptr_t DefinedFunction::operator==(objptr_t b)
{
    if (isinstance<DefinedFunction>(b))
    {
        // 表达式和参数一样它们才算相同
        DefinedFunction *t = dynamic_cast<DefinedFunction *>(b.get());
        if (t->args.size() != this->args.size())
            return to_boolean(false);
        if (t->sections != this->sections)
            return to_boolean(false);
        for (size_t i = 0; i < this->args.size(); i++)
        {
            if (isinstance<False>(this->args[i] == t->args[i]))
                return to_boolean(false);
        }
        return to_boolean(true);
    }
    return Function::operator==(b);
}

objptr_t DefinedFunction::operator()(funcargs_t args)
{
    funcargs_t n_args(this->args);
    for (size_t i = 0; i < (*sections)[0].args.size(); i++)
        n_args[i] = n_args[i]->replace((*sections)[0].args[i], args[i]);
    bool flag = true;
    for (size_t i = 0; i < this->sections->size(); i++)
    {
        boolptr_t contain;
        if (n_args.size() == 1)
            contain = (*this->sections)[i].domain->contains(n_args[0]);
        else
            contain = (*this->sections)[i].domain->contains(Tuple(n_args));
        if (isinstance<True>(contain))
            return (*this->sections)[i](n_args);
        if (!isinstance<False>(contain))
            flag = false;
    }
    if (flag)
        throw std::runtime_error("超出定义域");
    if (isinstance<ExprDefFunction>(this))
        return objptr_t(new ExprDefFunction(this->name, n_args, sections, this->domain, this->range));
    return objptr_t(new DefinedFunction(this->name, n_args, sections, this->domain, this->range));
}

objptr_t DefinedFunction::replace(objptr_t old, objptr_t _new)
{
    funcargs_t n_args(this->args);
    for (size_t i = 0; i < (*this->sections)[0].args.size(); i++)
        n_args[i] = n_args[i]->replace(old, _new);
    if (isinstance<ExprDefFunction>(this))
        return objptr_t(new ExprDefFunction(this->name, n_args, sections, this->domain, this->range));
    return objptr_t(new DefinedFunction(this->name, n_args, sections, this->domain, this->range));
}