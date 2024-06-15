#include <iostream>

#include "Lambda.h"
#include "Tuple.h"
#include "True.h"

Lambda::Lambda(lambdaargs_t args, funcbodyptr_t body, setptr_t domain, setptr_t range)
    : Function(domain, range)
{
    this->args = args;
    this->body = body;
}

std::string Lambda::toString()
{
    std::string result = "";
    if (this->args.size() > 1)
        result += "(";
    for (size_t i = 0; i < this->args.size(); i++)
    {
        result += this->args[i]->toString();
        if (i < this->args.size() - 1)
            result += ", ";
    }
    if (this->args.size() > 1)
        result += ")";
    result += " |-> ";
    result += this->body->toString();
    return result;
}

prettystring_t Lambda::toPrettyString()
{
    prettystring_t result;

    std::string args_str = "";
    if (this->args.size() > 1)
        args_str += "(";
    for (size_t i = 0; i < this->args.size(); i++)
    {
        args_str += this->args[i]->toString();
        if (i < this->args.size() - 1)
            args_str += ", ";
    }
    if (this->args.size() > 1)
        args_str += ")";

    args_str += " |-> ";

    result.push_back(args_str);

    prettystring_t body_str = this->body->toPrettyString();
    size_t d = (body_str.size() - result.size() + 1) / 2;
    for (size_t i = 0; i < d; i++)
    {
        result.insert(result.begin(), std::string(result[0].size(), ' '));
        result.insert(result.end(), std::string(result[0].size(), ' '));
    }
    for (size_t i = 0; i < body_str.size(); i++)
        result[i] += body_str[i];

    return normalize(result);
}

objptr_t Lambda::operator()(funcargs_t args)
{
    if (isinstance<False>(this->inDomain(args)))
        throw std::runtime_error("超出定义域");
    objptr_t result = this->body;
    for (size_t i = 0; i < this->args.size(); i++)
        result = result->replace(this->args[i], args[i]);
    return result;
}