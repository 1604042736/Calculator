#include <stdexcept>

#include "SetMapping.h"
#include "SetUndefFunction.h"

#include "True.h"
#include "False.h"
#include "Tuple.h"

objptr_t SetMapping::operator()(funcargs_t args)
{
    if (isinstance<False>(this->inDomain(args)))
        throw std::runtime_error("超出定义域");
    return funcptr_t(new SetUndefFunction(this->name, args, this->domain, this->range));
}