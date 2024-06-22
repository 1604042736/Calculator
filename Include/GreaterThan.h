#pragma once

#include "Relation.h"

/*大于*/
class GreaterThan : public Relation
{
public:
    GreaterThan(objptr_t lhs, objptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new GreaterThan(*this); }

    virtual boolptr_t operator!();

    virtual boolptr_t _simplify() { return ::simplify(this->lhs) >= ::simplify(this->rhs); }

    virtual std::string getRelationStr() { return ">="; }
};