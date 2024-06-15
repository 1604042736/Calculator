#include "SymDefAST.h"
#include "Error.h"
#include "ExprSymbol.h"
#include "BoolSymbol.h"
#include "SetSymbol.h"

objptr_t SymDefAST::exec(Runtime *runtime)
{
    objptr_t (*f)(std::string);
    if (runtime->flags.test(EXPR_FLAG))
    {
        f = [](std::string name)
        { return objptr_t(new ExprSymbol(name)); };
    }
    else if (runtime->flags.test(BOOL_FLAG))
    {
        f = [](std::string name)
        { return objptr_t(new BoolSymbol(name)); };
    }
    else if (runtime->flags.test(SET_FLAG))
    {
        f = [](std::string name)
        { return objptr_t(new SetSymbol(name)); };
    }
    else
    {
        f = [](std::string name)
        { return objptr_t(new Symbol(name)); };
    }
    for (size_t i = 0; i < this->names.size(); i++)
        runtime->defName(this->names[i], f(this->names[i]));
    return nullptr;
}