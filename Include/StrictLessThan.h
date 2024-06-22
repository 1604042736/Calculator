#pragma once

#include "Relation.h"

class Equality;
/*严格小于*/
class StrictLessThan : public Relation
{
public:
    StrictLessThan(objptr_t lhs, objptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new StrictLessThan(*this); }

    virtual boolptr_t operator||(Equality);
    virtual boolptr_t operator||(boolptr_t);
    virtual boolptr_t operator!();

    virtual boolptr_t _simplify() { return ::simplify(this->lhs) < ::simplify(this->rhs); }

    virtual std::string getRelationStr() { return "<"; }
};