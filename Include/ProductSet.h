#pragma once

#include "SetOp.h"
#include "Integer.h"

class ProductSet : public SetOp
{
public:
    ProductSet(setopargs_t args) : SetOp(args) {}
    ProductSet(setptr_t, Integer);

    virtual Object *copyThis() { return new ProductSet(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual boolptr_t contains(objptr_t element);

    virtual setptr_t _simplify();

    virtual std::string getOpStr() { return "×"; }
};