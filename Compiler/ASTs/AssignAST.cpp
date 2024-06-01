#include "AssignAST.h"

objptr_t AssignAST::exec(Runtime *runtime)
{
    objptr_t expr = this->children[0]->exec(runtime);
    runtime->defName(this->name, expr);
    return nullptr;
}