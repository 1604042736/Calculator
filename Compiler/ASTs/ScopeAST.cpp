#include "ScopeAST.h"
#include "Error.h"

objptr_t ScopeAST::exec(Runtime *runtime)
{
    runtime->enterScope();
    this->children[0]->exec(runtime);
    scope_t scope = runtime->curScope();
    runtime->leaveScope();

    for (size_t i = 0; i < this->names.size(); i++)
    {
        if (scope.count(this->names[i]) != 0)
            runtime->defName(this->names[i], scope[this->names[i]]);
        else
            throw NameError(this->names[i], this->context);
    }

    return nullptr;
}