#include <stdexcept>

#include "Mapping.h"
#include "Tuple.h"
#include "False.h"
#include "RealSet.h"
#include "UndefinedFunction.h"
#include "Common.h"

objptr_t Mapping::operator()(funcargs_t args)
{
    if (args.size() == 1)
    {
        if (isinstance<False>(this->domain->contains(args[0])))
            throw std::runtime_error("超出定义域");
    }
    else
    {
        if (isinstance<False>(this->domain->contains(Tuple(args))))
            throw std::runtime_error("超出定义域");
    }
    return funcptr_t(new UndefinedFunction(this->name, args, this->domain, this->range));
}