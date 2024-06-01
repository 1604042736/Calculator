#include "TupleAST.h"
#include "Tuple.h"

objptr_t TupleAST::exec(Runtime *runtime)
{
    tupleargs_t args;
    for (size_t i = 0; i < this->children.size(); i++)
    {
        objptr_t a = this->children[i]->exec(runtime);
        if (a != nullptr)
            args.push_back(a);
    }
    return objptr_t(new Tuple(args));
}