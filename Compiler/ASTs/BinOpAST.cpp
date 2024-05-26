#include <stdexcept>

#include "BinOpAST.h"
#include "Error.h"

objptr_t BinOpAST::exec(Runtime *runtime)
{
    objptr_t l = this->children[0]->exec(runtime);
    objptr_t r = this->children[1]->exec(runtime);
    if (this->op == "+")
        return l + r;
    else if (this->op == "-")
        return l - r;
    else if (this->op == "*")
        return l * r;
    else if (this->op == "/")
        return l / r;
    else if (this->op == "^")
        return l->pow(r);
    else
        throw Error("Unexpected operator: " + this->op, this->context);
}