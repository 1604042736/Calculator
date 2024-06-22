#include "Equality.h"
#include "Unequality.h"
#include "GreaterThan.h"
#include "LessThan.h"
#include "EnumSet.h"
#include "Tuple.h"
#include "True.h"
#include "False.h"
#include "EmptySet.h"

boolptr_t Equality::operator||(StrictGreaterThan b)
{
    if (this->lhs == b.lhs && this->rhs == b.rhs)
        return boolptr_t(new GreaterThan(this->lhs, this->rhs));
    return Relation::operator||(b.copyToBoolPtr());
}

boolptr_t Equality::operator||(StrictLessThan b)
{
    if (this->lhs == b.lhs && this->rhs == b.rhs)
        return boolptr_t(new LessThan(this->lhs, this->rhs));
    return Relation::operator||(b.copyToBoolPtr());
}

boolptr_t Equality::operator||(boolptr_t b)
{
    if (isinstance<StrictGreaterThan>(b))
        return (*this) || *(StrictGreaterThan *)b.get();
    if (isinstance<StrictLessThan>(b))
        return (*this) || *(StrictLessThan *)b.get();
    return Relation::operator||(b);
}

boolptr_t Equality::operator!()
{
    return boolptr_t(new Unequality(this->lhs, this->rhs));
}

boolptr_t Equality::_simplify()
{
    return ::simplify(this->lhs) == ::simplify(this->rhs);
}

setptr_t Equality::belongto()
{
    setptr_t a = this->lhs->belongto();
    setptr_t b = this->rhs->belongto();
    if (isinstance<True>(a->operator==(b))) // T,T
        return setptr_t(new EnumSet({objptr_t(new Tuple({objptr_t(new True()),
                                                         objptr_t(new True())}))}));
    else if (isinstance<EmptySet>(a->operator&(b))) // F,F
        return setptr_t(new EnumSet({objptr_t(new Tuple({objptr_t(new False()),
                                                         objptr_t(new False())}))}));
    else // F,T
        return setptr_t(new EnumSet({objptr_t(new Tuple({objptr_t(new False()),
                                                         objptr_t(new True())}))}));
}