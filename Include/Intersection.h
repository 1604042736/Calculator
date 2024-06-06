#pragma once

#include "SetOp.h"

/*交集*/
class Intersection : public SetOp
{
public:
    Intersection(setopargs_t args) : SetOp(args) {}

    virtual Object *copyThis() { return new Intersection(*this); }

    virtual boolptr_t contains(objptr_t element);

    virtual std::string getOpStr() { return "∩"; }
};