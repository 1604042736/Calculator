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