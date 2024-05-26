#pragma once

#include "Relation.h"
#include "StrictGreaterThan.h"
#include "StrictLessThan.h"

/*等于*/
class Equality : public Relation
{
public:
    Equality(exprptr_t lhs, exprptr_t rhs) : Relation(lhs, rhs) {}

    virtual Object *copyThis() { return new Equality(*this); }

    virtual boolptr_t operator||(StrictGreaterThan);
    virtual boolptr_t operator||(StrictLessThan);
    virtual boolptr_t operator||(boolptr_t);
    virtual boolptr_t operator!();

    virtual boolptr_t _simplify();

    virtual std::string getRelationStr() { return "="; }
};