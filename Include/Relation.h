#pragma once

#include "Boolean.h"
#include "Expression.h"

/*关系运算*/
class Relation : public Boolean
{
public:
    Relation(objptr_t lhs, objptr_t rhs) : lhs(lhs), rhs(rhs) {}

    virtual std::string toString() { return this->lhs->toString() + " " + this->getRelationStr() + " " + this->rhs->toString(); }
    virtual prettystring_t toPrettyString();

    virtual boolptr_t operator==(boolptr_t);

    virtual std::string getRelationStr() = 0;

    objptr_t lhs, rhs; // 左右操作符
};