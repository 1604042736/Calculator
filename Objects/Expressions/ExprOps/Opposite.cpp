#include "Opposite.h"
#include "Common.h"

exprptr_t Opposite::opposite()
{
    expropargs_t result;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (isinstance<Integer>(this->args[i]))
        {
            Integer a = *(Integer *)this->args[i].get();
            if (a == -1)
                continue;
        }
        result.push_back(this->args[i]);
    }
    if (result.size() == 1)
        return result[0];
    return exprptr_t(new Mul(result));
}

std::string Opposite::toString()
{
    return " - " + this->opposite()->toString();
}

prettystring_t Opposite::toPrettyString()
{
    prettystring_t result(this->opposite()->toPrettyString());
    for (size_t i = 0; i < result.size(); i++)
    {
        if (i == result.size() / 2)
            result[i] = " - " + result[i];
        else
            result[i] = "   " + result[i];
    }
    return result;
}

exprptr_t Opposite::_simplify()
{
    return this->opposite()->simplify()->opposite();
}

exprptr_t Opposite::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    return this->opposite()->eval(keep)->opposite();
}