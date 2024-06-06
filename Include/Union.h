#pragma once

#include "SetOp.h"

/*并集*/
class Union : public SetOp
{
public:
    Union(setopargs_t args) : SetOp(args) {}

    virtual Object *copyThis() { return new Union(*this); }

    virtual boolptr_t contains(objptr_t element);

    virtual std::string getOpStr() { return "∪"; }
};