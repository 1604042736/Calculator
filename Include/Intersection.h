#pragma once

#include "SetOp.h"

/*交集*/
class Intersection : public SetOp
{
public:
    Intersection(setopargs_t args) : SetOp(args) {}

    virtual Object *copyThis() { return new Intersection(*this); }

    virtual std::string getOpStr() { return "∩"; }
};