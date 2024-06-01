#pragma once

#include "Logic.h"

/*或*/
class Or : public Logic
{
public:
    Or(logicargs_t args) : Logic(args) {}

    virtual Object *copyThis() { return new Or(*this); }

    virtual boolptr_t operator||(Or);
    virtual boolptr_t operator||(boolptr_t);
    virtual boolptr_t operator!();

    virtual boolptr_t _simplify();

    virtual std::string getLogicStr() { return "∨"; }

    virtual size_t getPriority() { return 1; }
};