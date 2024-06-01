#include <stdexcept>

#include "NameAST.h"
#include "ExprSymbol.h"
#include "Error.h"

objptr_t NameAST::exec(Runtime *runtime)
{
    objptr_t obj = runtime->findName(this->name);
    if (obj == nullptr)
        throw NameError(this->name, this->context);
    return obj;
}