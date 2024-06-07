#include <stdexcept>

#include "Sgn.h"
#include "Common.h"
#include "Integer.h"
#include "True.h"

exprptr_t Sgn::_simplify()
{
    if (isinstance<True>(this->arg > Integer(0)))
        return exprptr_t(new Integer(1));
    else if (isinstance<True>(this->arg == Integer(0)))
        return exprptr_t(new Integer(0));
    else if (isinstance<True>(this->arg < Integer(0)))
        return exprptr_t(new Integer(-1));
    return Expression::_simplify();
}

boolptr_t Sgn::operator==(exprptr_t b)
{
    if (isinstance<Sgn>(b))
        return this->arg == dynamic_cast<Sgn *>(b.get())->arg;
    return to_boolean(false);
}

objptr_t Sgn::replace(objptr_t old, objptr_t _new)
{
    objptr_t t = this->arg->replace(old, _new);
    if (!isinstance<Expression>(t))
        throw std::runtime_error("[Sgn::replace]超出定义域");

    return exprptr_t(new Sgn(dynamic_cast<Expression *>(t.get())->copyToExprPtr()));
}