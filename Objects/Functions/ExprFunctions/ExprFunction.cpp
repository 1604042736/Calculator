#include "ExprFunction.h"

boolptr_t ExprFunction::operator==(exprptr_t b)
{
    if (isinstance<Function>(b))
    {
        Function *f = dynamic_cast<Function *>(b.get());
        if (this->args.size() != f->args.size())
            return to_boolean(false);
        boolptr_t result = to_boolean(this->name == f->name);
        for (size_t i = 0; i < args.size(); i++)
            result = result->operator&&(args[i] == f->args[i]);
        return result;
    }
    return Expression::operator==(b);
}

exprptr_t ExprFunction::diff(exprptr_t target)
{
    if (this->args.size() > 1)
        throw std::runtime_error("暂不支持对多参函数求导");
    exprptr_t result = Expression::diff(target);
    if (isinstance<Expression>(args[0]))
        result = result * dynamic_cast<Expression *>(this->args[0].get())->diff(target);
    return result;
}