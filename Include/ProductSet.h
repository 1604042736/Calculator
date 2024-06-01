#pragma once

#include "SetOp.h"

class ProductSet : public SetOp
{
public:
    ProductSet(setopargs_t args) : SetOp(args) {}

    virtual Object *copyThis() { return new ProductSet(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual std::string getOpStr() { return "×"; }
};