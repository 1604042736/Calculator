#include <stdexcept>

#include "Mapping.h"
#include "Tuple.h"
#include "False.h"
#include "RealSet.h"
#include "UndefinedFunction.h"

#include "ExprUndefFunction.h"

objptr_t Mapping::operator()(funcargs_t args)
{
    if (isinstance<False>(this->inDomain(args)))
        throw std::runtime_error("超出定义域");
    if (isinstance<True>(RealSet().includes(this->range)))
        return funcptr_t(new ExprUndefFunction(this->name, args, this->domain, this->range));
    return funcptr_t(new UndefinedFunction(this->name, args, this->domain, this->range));
}