#pragma once

#include "Object.h"

class Tuple;
typedef std::shared_ptr<Tuple> tupleptr_t;
typedef std::vector<objptr_t> tupleargs_t;

/*元组*/
class Tuple : public Object
{
public:
    Tuple(tupleargs_t args) : args(args) {}

    virtual Object *copyThis() { return new Tuple(*this); }

    virtual std::string toString();

    virtual boolptr_t operator==(Tuple);
    virtual boolptr_t operator==(objptr_t);

    tupleargs_t args;
};