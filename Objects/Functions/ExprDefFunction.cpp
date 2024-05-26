#include <stdexcept>
#include <iostream>

#include "ExprDefFunction.h"
#include "Tuple.h"
#include "Common.h"

exprptr_t ExprDefFunction::_simplify()
{
    funcargs_t args(this->args);
    for (size_t i = 0; i < args.size(); i++)
    {
        if (isinstance<Expression>(args[i]))
            args[i] = dynamic_cast<Expression *>(args[i].get())->simplify();
    }
    bool flag = true;
    for (size_t i = 0; i < this->sections->size(); i++)
    {
        boolptr_t contain;
        if (args.size() == 1)
            contain = (*this->sections)[i].domain->contains(args[0]);
        else
            contain = (*this->sections)[i].domain->contains(Tuple(args));
        if (isinstance<True>(contain))
            return dynamic_cast<Expression *>((*this->sections)[i](args).get())->copyToExprPtr();
        if (!isinstance<False>(contain))
            flag = false;
    }
    if (flag)
        throw std::runtime_error("超出定义域");
    return exprptr_t(new ExprDefFunction(this->name, args, this->sections, DefinedFunction::domain, DefinedFunction::range));
}