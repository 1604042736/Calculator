#include <stdexcept>

#include "ExprMapping.h"
#include "ExprFunction.h"

objptr_t ExprMapping::operator()(funcargs_t args)
{
    objptr_t t = Mapping::operator()(args);
    if (isinstance<Function>(t))
    {
        Function *f = dynamic_cast<Function *>(t.get());
        if (f->mapping != nullptr)
            t = objptr_t(new ExprFunction(f->mapping, f->args));
    }
    return t;
}