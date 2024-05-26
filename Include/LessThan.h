#pragma once

#include "Relation.h"

/*小于*/
class LessThan : public Relation
{
public:
    LessThan(exprptr_t lhs, exprptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new LessThan(*this); }

    virtual boolptr_t operator!();

    virtual boolptr_t _simplify() { return this->lhs->simplify() <= this->rhs->simplify(); }

    virtual std::string getRelationStr() { return "<="; }
};