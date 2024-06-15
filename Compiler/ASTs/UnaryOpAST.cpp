#include "UnaryOpAST.h"
#include "Error.h"

#include "Opposite.h"
#include "Not.h"

exprptr_t opposite(exprptr_t a)
{
    return exprptr_t(new Opposite(a));
}

objptr_t opposite(objptr_t a)
{
    if (isinstance<Expression>(a))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        return opposite(ea);
    }
    throw std::runtime_error("'" + std::string(typeid(*a.get()).name()) + "'不支持一元运算符: -");
}

objptr_t _not(objptr_t a)
{
    if (isinstance<Boolean>(a))
    {
        boolptr_t ba = dynamic_cast<Boolean *>(a.get())->copyToBoolPtr();
        return boolptr_t(new Not(ba));
    }
    return a->operator!();
}

objptr_t UnaryOpAST::exec(Runtime *runtime)
{
    objptr_t expr = this->children[0]->exec(runtime);
    try
    {
        if (this->op == "-")
            return opposite(expr);
        else if (this->op == "not")
            return _not(expr);
        else
            throw Error("意料之外的一元运算符: " + this->op, this->context);
    }
    catch (std::exception &e)
    {
        throw Error(e.what(), this->context);
    }
}