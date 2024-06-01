#include "Not.h"

std::string Not::toString()
{
    if (this->args[0]->getPriority() < this->getPriority())
        return this->getLogicStr() + "(" + this->args[0]->toString() + ")";
    else
        return this->getLogicStr() + this->args[0]->toString();
}

prettystring_t Not::toPrettyString()
{
    prettystring_t arg_str = this->args[0]->toPrettyString();
    prettystring_t result(arg_str.size(), "");

    for (size_t i = 0; i < result.size(); i++)
    {
        if (i == result.size() / 2)
            result[i] += this->getLogicStr();
        else
            result[i] += " ";
    }
    bool flag = this->args[0]->getPriority() < this->getPriority();
    if (flag)
    {
        for (size_t i = 0; i < result.size(); i++)
            result[i] += "(";
    }

    for (size_t i = 0; i < arg_str.size(); i++)
        result[i] += arg_str[i];

    if (flag)
    {
        for (size_t i = 0; i < result.size(); i++)
            result[i] += ")";
    }

    return normalize(result);
}

boolptr_t Not::operator!()
{
    return this->args[0];
}

boolptr_t Not::_simplify()
{
    return !this->args[0]->simplify();
}