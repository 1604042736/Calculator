#pragma once

#include "Set.h"

typedef std::vector<setptr_t> setopargs_t;

/*集合运算*/
class SetOp : public Set
{
public:
    SetOp(setopargs_t args) : args(args) {}

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual std::string getOpStr() = 0;

    setopargs_t args;
};