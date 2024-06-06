#include <stdexcept>

#include "Sqrt.h"
#include "Float.h"
#include "False.h"
#include "Common.h"

Sqrt::Sqrt(exprptr_t arg) : Pow({arg, std::shared_ptr<Float>(new Float(0.5))})
{
    this->arg = arg;
}

std::string Sqrt::toString()
{
    return "sqrt(" + this->arg->toString() + ")";
}

prettystring_t Sqrt::toPrettyString()
{
    prettystring_t arg_str = this->arg->toPrettyString();
    size_t max_len = arg_str[0].size() + 1;

    prettystring_t result(arg_str.size(), " |");
    result.back() = "\\|";
    result.insert(result.begin(), " " + std::string(max_len, '-'));
    for (size_t i = 0; i < arg_str.size(); i++)
        result[i + 1] += arg_str[i];

    return normalize(result);
}

std::string Sqrt::toLateX()
{
    return "\\sqrt{" + this->arg->toLateX() + "}";
}

exprptr_t Sqrt::_simplify()
{
    return this->arg->simplify()->sqrt();
}

exprptr_t Sqrt::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    return this->arg->eval(keep)->sqrt(keep);
}

exprptr_t SqrtMapping::operator()(exprptr_t arg)
{
    return arg->sqrt();
}

objptr_t SqrtMapping::operator()(funcargs_t args)
{
    if (args.size() != 1)
        throw std::runtime_error("Too many args");
    Expression *arg = dynamic_cast<Expression *>(args[0].get());
    if (arg == nullptr || isinstance<False>(arg->copyToExprPtr() >= Integer(0)))
        throw std::runtime_error("超出定义域");
    return this->operator()(arg->copyToExprPtr());
}