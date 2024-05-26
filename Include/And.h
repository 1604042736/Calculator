#pragma once

#include "Logic.h"

/*与*/
class And : public Logic
{
public:
    And(logicargs_t args) : Logic(args) {}

    virtual Object *copyThis() { return new And(*this); }

    virtual boolptr_t operator&&(And);
    virtual boolptr_t operator&&(boolptr_t);
    virtual boolptr_t operator!();

    virtual boolptr_t _simplify();

    virtual std::string getLogicStr() { return "&"; }
    virtual size_t getPriority() { return 2; }
};