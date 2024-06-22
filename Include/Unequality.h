#pragma once

#include "Relation.h"

/*不等于*/
class Unequality : public Relation
{
public:
    Unequality(objptr_t lhs, objptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new Unequality(*this); }

    virtual boolptr_t operator!();

    virtual boolptr_t _simplify();

    virtual std::string getRelationStr() { return "!="; }
};