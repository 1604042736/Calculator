#include "SymDefAST.h"
#include "Error.h"
#include "ExprSymbol.h"

objptr_t SymDefAST::exec(Runtime *runtime)
{
    objptr_t (*f)(std::string);
    if (runtime->flags.test(EXPR_FLAG))
    {
        f = [](std::string name)
        { return objptr_t(new ExprSymbol(name)); };
    }
    else
        throw Error("无法确定如何定义符号", this->context);
    for (size_t i = 0; i < this->names.size(); i++)
        runtime->defName(this->names[i], f(this->names[i]));
    return nullptr;
}