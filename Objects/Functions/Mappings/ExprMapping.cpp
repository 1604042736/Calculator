#include <stdexcept>

#include "ExprMapping.h"
#include "ExprUndefFunction.h"
#include "Common.h"
#include "True.h"
#include "False.h"
#include "Tuple.h"

objptr_t ExprMapping::operator()(funcargs_t args)
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
    return funcptr_t(new ExprUndefFunction(this->name, args, this->domain, this->range));
}