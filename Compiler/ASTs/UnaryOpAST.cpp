#include "UnaryOpAST.h"
#include "Error.h"
#include "Common.h"

objptr_t UnaryOpAST::exec(Runtime *runtime)
{
    objptr_t expr = this->children[0]->exec(runtime);
    if (this->op == "-")
    {
        if (isinstance<Expression>(expr))
            return dynamic_cast<Expression *>(expr.get())->opposite();
        throw Error("'" + std::string(typeid(*expr.get()).name()) + "'不支持一元运算符: " + this->op, this->context);
    }
    else if (this->op == "not")
        return expr->operator!();
    else
        throw Error("意料之外的一元运算符: " + this->op, this->context);
}