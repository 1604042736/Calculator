#pragma once

#include "Symbol.h"
#include "Set.h"

class SetSymbol : public Symbol, virtual public Set
{
public:
    SetSymbol(std::string name) : Symbol(name) {}

    virtual Object *copyThis() { return new SetSymbol(*this); }
};