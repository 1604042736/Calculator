#include <stdexcept>

#include "ExprMapping.h"
#include "ExprUndefFunction.h"

#include "True.h"
#include "False.h"
#include "Tuple.h"

objptr_t ExprMapping::operator()(funcargs_t args)
{
    if (isinstance<False>(this->inDomain(args)))
        throw std::runtime_error("超出定义域");
    return funcptr_t(new ExprUndefFunction(this->name, args, this->domain, this->range));
}