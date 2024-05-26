#include "CompoundAST.h"

objptr_t CompoundAST::exec(Runtime *runtime)
{
    for (size_t i = 0; i < this->children.size(); i++)
        this->children[i]->exec(runtime);
    return nullptr;
}