#pragma once

#include <string>

#include "Boolean.h"
#include "Symbol.h"

/*表示逻辑表达式的符号*/
class BoolSymbol : public Boolean, public Symbol
{
public:
    BoolSymbol(std::string name) : Symbol(name) {}

    virtual Object *copyThis() { return new BoolSymbol(*this); }
};