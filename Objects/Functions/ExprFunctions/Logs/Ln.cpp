#include <stdexcept>

#include "Ln.h"
#include "Derivative.h"
#include "True.h"

exprptr_t Ln::diff(exprptr_t target)
{
    if (!isinstance<Symbol>(target))
        return ExprFunction::diff(target);
    return this->tnum->reciprocal() * this->tnum->diff(target);
}

objptr_t LnMapping::operator()(funcargs_t args)
{
    if (args.size() != 1 || !isinstance<Expression>(args[0]))
        throw std::runtime_error("[LgMapping]超出定义域");
    return exprptr_t(new Ln(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr()));
}