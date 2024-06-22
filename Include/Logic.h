#pragma once

#include "Boolean.h"

typedef std::vector<boolptr_t> logicargs_t;

/*逻辑运算*/
class Logic : public Boolean
{
public:
    Logic(logicargs_t args) : args(args) { this->sortArgs(); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual boolptr_t operator==(boolptr_t);

    virtual void sortArgs();

    virtual std::string getLogicStr() = 0;

    logicargs_t args;
};