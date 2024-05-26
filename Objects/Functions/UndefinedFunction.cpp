#include "UndefinedFunction.h"

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