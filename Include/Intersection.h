#pragma once

#include "SetOp.h"

/*交集*/
class Intersection : public SetOp
{
public:
    Intersection(setopargs_t args) : SetOp(args) {}

    virtual Object *copyThis() { return new Intersection(*this); }

    virtual boolptr_t contains(objptr_t element);

    virtual setptr_t _simplify();

    virtual std::string getOpStr() { return "∩"; }

    virtual objptr_t replace(objptr_t, objptr_t);
};