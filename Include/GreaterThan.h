#pragma once

#include "Relation.h"

/*大于*/
class GreaterThan : public Relation
{
public:
    GreaterThan(exprptr_t lhs, exprptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new GreaterThan(*this); }

    virtual boolptr_t operator!();

    virtual boolptr_t _simplify() { return this->lhs->simplify() >= this->rhs->simplify(); }

    virtual std::string getRelationStr() { return ">="; }
};