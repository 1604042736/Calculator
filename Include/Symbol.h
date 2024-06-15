#pragma once

#include "Object.h"

/*符号*/
class Symbol : virtual public Object
{
public:
    Symbol(std::string name) : name(name) {}

    virtual Object *copyThis() { return new Symbol(*this); }

    virtual std::string toString() { return this->name; }

    std::string name;
};

typedef std::shared_ptr<Symbol> symptr_t;