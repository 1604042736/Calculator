#include "CallAST.h"
#include "Error.h"
#include "Function.h"

objptr_t CallAST::exec(Runtime *runtime)
{
    objptr_t func = this->func->exec(runtime);
    funcargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
        args.push_back(this->args[i]->exec(runtime));
    try
    {
        return func->operator()(args);
    }
    catch (std::exception &e)
    {
        throw Error(e.what(), context);
    }
}