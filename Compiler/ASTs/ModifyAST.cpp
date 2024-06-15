#include "ModifyAST.h"
#include "Error.h"

objptr_t ModifyAST::exec(Runtime *runtime)
{
    flags_t _flags = runtime->flags;
    if (this->mode == "expr")
        runtime->flags[EXPR_FLAG] = 1;
    else if (this->mode == "bool")
        runtime->flags[BOOL_FLAG] = 1;
    else if (this->mode == "set")
        runtime->flags[SET_FLAG] = 1;
    else
        throw Error("未知的修改模式: " + this->mode, this->context);

    objptr_t ret = this->target->exec(runtime);

    runtime->flags = _flags;
    return ret;
}