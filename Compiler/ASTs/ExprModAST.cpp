#include "ExprModAST.h"

objptr_t ExprModAST::exec(Runtime *runtime)
{
    flags_t _flags = runtime->flags;
    runtime->flags[EXPR_FLAG] = 1;
    objptr_t ret = this->target->exec(runtime);
    runtime->flags = _flags;
    return ret;
}