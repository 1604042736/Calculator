#include "IntAST.h"
#include "Integer.h"

objptr_t IntAST::exec(Runtime *)
{
    return objptr_t(new Integer(this->num));
}