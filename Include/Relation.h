#pragma once

#include "Boolean.h"
#include "Expression.h"

/*关系运算, 主要针对Expression*/
class Relation : public Boolean
{
public:
    Relation(exprptr_t lhs, exprptr_t rhs) : lhs(lhs), rhs(rhs) {}

    virtual std::string toString() { return this->lhs->toString() + " " + this->getRelationStr() + " " + this->rhs->toString(); }
    virtual prettystring_t toPrettyString();

    virtual bool operator==(boolptr_t);

    virtual std::string getRelationStr() = 0;

    exprptr_t lhs, rhs; // 左右操作符
};