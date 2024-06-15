#include <stdexcept>

#include "SetMapping.h"
#include "SetFunction.h"

objptr_t SetMapping::operator()(funcargs_t args)
{
    objptr_t t = Mapping::operator()(args);
    if (isinstance<Function>(t))
    {
        Function *f = dynamic_cast<Function *>(t.get());
        if (f->mapping != nullptr)
            t = objptr_t(new SetFunction(f->mapping, f->args));
    }
    return t;
}