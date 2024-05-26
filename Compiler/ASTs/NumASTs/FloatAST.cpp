#include "FloatAST.h"
#include "Float.h"

objptr_t FloatAST::exec(Runtime *)
{
    return objptr_t(new Float(this->num));
}