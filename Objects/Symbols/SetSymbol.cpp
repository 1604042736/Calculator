#include "SetSymbol.h"

boolptr_t SetSymbol::operator==(setptr_t b)
{
    Symbol *symbol = dynamic_cast<Symbol *>(b.get());
    if (symbol != nullptr)
    {
        if (this->name == symbol->name)
            return to_boolean(true);
        else
            return to_boolean(false);
    }
    return Set::operator==(b);
}