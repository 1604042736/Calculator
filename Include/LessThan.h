#pragma once

#include "Relation.h"

/*小于*/
class LessThan : public Relation
{
public:
    LessThan(objptr_t lhs, objptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new LessThan(*this); }

    virtual boolptr_t operator!();

    virtual boolptr_t _simplify() { return ::simplify(this->lhs) <= ::simplify(this->rhs); }

    virtual std::string getRelationStr() { return "<="; }
};