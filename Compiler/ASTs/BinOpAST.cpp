#include <stdexcept>

#include "BinOpAST.h"
#include "Error.h"

objptr_t BinOpAST::exec(Runtime *runtime)
{
    objptr_t l = this->children[0]->exec(runtime);
    objptr_t r = this->children[1]->exec(runtime);
    try
    {
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
        else if (this->op == "and")
            return l && r;
        else if (this->op == "or")
            return l || r;
        else if (this->op == "=")
            return l == r;
        else if (this->op == "!=")
            return l != r;
        else if (this->op == ">")
            return l > r;
        else if (this->op == ">=")
            return l >= r;
        else if (this->op == "<")
            return l < r;
        else if (this->op == "<=")
            return l <= r;
        else
            throw Error("意料之外的二元运算符: " + this->op, this->context);
    }
    catch (std::exception &e)
    {
        throw Error(e.what(), this->context);
    }
}