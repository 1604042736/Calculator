#include <stdexcept>

#include "Abs.h"
#include "Interval.h"
#include "Integer.h"
#include "Infinity.h"
#include "Sgn.h"

Abs::Abs(exprptr_t arg) : arg(arg), ExprFunction("abs", {arg})
{
    this->mapping = mappingptr_t(new AbsMapping());
}

std::string Abs::toString()
{
    return "|" + this->arg->toString() + "|";
}

prettystring_t Abs::toPrettyString()
{
    prettystring_t result = this->arg->toPrettyString();
    for (size_t i = 0; i < result.size(); i++)
        result[i] = "|" + result[i] + "|";
    return normalize(result);
}

exprptr_t Abs::diff(exprptr_t target)
{
    return (this->arg * Sgn(this->arg))->diff(target);
}

exprptr_t AbsMapping::operator()(exprptr_t arg)
{
    return arg->abs();
}

objptr_t AbsMapping::operator()(funcargs_t args)
{
    if (args.size() != 1 || !isinstance<Expression>(args[0]))
        throw std::runtime_error("超出定义域");
    return this->operator()(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr());
}