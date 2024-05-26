#pragma once

#include "Logic.h"

/*非*/
class Not : public Logic
{
public:
    // 非运算一定只有一个操作数
    Not(boolptr_t arg) : Logic({arg}) {}

    virtual Object *copyThis() { return new Not(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual boolptr_t operator!();

    virtual boolptr_t _simplify();

    virtual std::string getLogicStr() { return "~"; }

    virtual size_t getPriority() { return 3; }
};