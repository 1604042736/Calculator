#include "UndefinedFunction.h"

#include "ExprUndefFunction.h"

UndefinedFunction::UndefinedFunction(std::string name, funcargs_t args, setptr_t domain, setptr_t range)
    : Function(domain, range)
{
    this->name = name;
    this->args = args;
}

std::string UndefinedFunction::toString()
{
    std::string result = this->name + "(";
    for (size_t i = 0; i < this->args.size(); i++)
    {
        result += this->args[i]->toString();
        if (i < this->args.size() - 1)
            result += ", ";
    }
    result += ")";
    return result;
}

prettystring_t UndefinedFunction::toPrettyString()
{
    prettystring_t result;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        prettystring_t arg_str = this->args[i]->toPrettyString();
        if (i == 0)
            result = arg_str;
        else
        {
            if (arg_str.size() > result.size())
            {
                size_t d = arg_str.size() - result.size();
                for (size_t j = 0; j < d; j++)
                    result.insert(result.begin(), std::string(result[0].size(), ' '));
            }
            for (size_t j = 0; j < arg_str.size(); j++)
                result[j] += arg_str[j];
        }
        if (i < this->args.size() - 1)
        {
            result.back() += ", ";
            result = normalize(result);
        }
    }

    for (size_t i = 0; i < result.size(); i++)
    {
        if (i == result.size() / 2)
            result[i] = this->name + "(" + result[i] + ")";
        else
            result[i] = std::string(this->name.size(), ' ') + "(" + result[i] + ")";
    }

    return normalize(result);
}

boolptr_t UndefinedFunction::operator==(objptr_t b)
{
    if (isinstance<UndefinedFunction>(b))
    {
        // 表达式和参数一样它们才算相同
        UndefinedFunction *t = dynamic_cast<UndefinedFunction *>(b.get());
        if (t->args.size() != this->args.size() || this->name != t->name)
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

objptr_t UndefinedFunction::replace(objptr_t old, objptr_t _new)
{
    funcargs_t n_args(this->args);
    for (size_t i = 0; i < n_args.size(); i++)
        n_args[i] = n_args[i]->replace(old, _new);
    if (isinstance<ExprUndefFunction>(this))
        return objptr_t(new ExprUndefFunction(this->name, n_args, this->domain, this->range));
    return objptr_t(new UndefinedFunction(this->name, n_args, this->domain, this->range));
}