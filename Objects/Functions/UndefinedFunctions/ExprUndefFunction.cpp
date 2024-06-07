#include <stdexcept>

#include "ExprUndefFunction.h"
#include "Common.h"

exprptr_t ExprUndefFunction::diff(exprptr_t target)
{
    exprptr_t result = Expression::diff(target);
    if (this->args.size() > 1)
        throw std::runtime_error("暂不支持对对参函数求导");
    if (isinstance<Expression>(this->args[0]))
        result = result * (dynamic_cast<Expression *>(this->args[0].get()))->diff(target);
    return result;
}