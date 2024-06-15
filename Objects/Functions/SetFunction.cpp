#include "SetFunction.h"

boolptr_t SetFunction::operator==(setptr_t b)
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
    return Set::operator==(b);
}