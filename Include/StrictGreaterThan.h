#pragma once

#include "Relation.h"

class Equality;
/*严格大于*/
class StrictGreaterThan : public Relation
{
public:
    StrictGreaterThan(exprptr_t lhs, exprptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new StrictGreaterThan(*this); }

    virtual boolptr_t operator||(Equality);
    virtual boolptr_t operator||(boolptr_t);
    virtual boolptr_t operator!();

    virtual boolptr_t _simplify() { return this->lhs->simplify() > this->rhs->simplify(); }

    virtual std::string getRelationStr() { return ">"; }
};