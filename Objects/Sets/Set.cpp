#include <stdexcept>

#include "Set.h"
#include "Contains.h"
#include "True.h"
#include "EmptySet.h"
#include "Expression.h"
#include "Union.h"
#include "Intersection.h"
#include "EnumSet.h"
#include "Includes.h"
#include "Common.h"
#include "ProductSet.h"
#include "Tuple.h"
#include "UniversalSet.h"

setptr_t Set::operator|(setptr_t b)
{
    if (isinstance<True>(this->contains(b)))
        return setptr_t(dynamic_cast<Set *>(this->copyThis()));
    if (isinstance<True>(b->contains(setptr_t(dynamic_cast<Set *>(this->copyThis())))))
        return b;
    return setptr_t(new Union({setptr_t(dynamic_cast<Set *>(this->copyThis())), b}));
}
objptr_t Set::operator|(objptr_t b)
{
    if (isinstance<Set>(b))
        return this->operator|(dynamic_cast<Set *>(b.get())->copyToSetPtr());
    return Object::operator|(b);
}

setptr_t Set::operator&(setptr_t b)
{
    if (isinstance<True>(this->contains(b)))
        return b;
    if (isinstance<True>(b->contains(this->copyToSetPtr())))
        return setptr_t(dynamic_cast<Set *>(this->copyThis()));
    return setptr_t(new Intersection({this->copyToSetPtr(), b}));
}
objptr_t Set::operator&(objptr_t b)
{
    if (isinstance<Set>(b))
        return this->operator&(dynamic_cast<Set *>(b.get())->copyToSetPtr());
    return Object::operator&(b);
}

boolptr_t Set::contains(objptr_t element)
{
    setptr_t belongset = element->belongto();
    boolptr_t b = this->includes(belongset);
    if (isinstance<True>(b))
        return b;
    return boolptr_t(new Contains(element, this->copyToSetPtr()));
}

setptr_t Set::product(setptr_t b)
{
    return setptr_t(new ProductSet({this->copyToSetPtr(), b}));
}

setptr_t Set::productpow(Integer b)
{
    if (b < 0)
        throw std::runtime_error("集合的pow运算指数必须>=0");
    if (b == 0)
        return setptr_t(new EnumSet({objptr_t(new Tuple({}))}));
    setopargs_t args;
    setptr_t self = this->copyToSetPtr();
    for (Integer i = 0; i < b; i = i + 1)
        args.push_back(self);
    return setptr_t(new ProductSet(args));
}

boolptr_t Set::contains(Object &&element)
{
    return this->contains(objptr_t(element.copyThis()));
}

boolptr_t Set::includes(setptr_t subset)
{
    if (isinstance<True>(*this == subset)) // 自己包含自己
        return boolptr_t(new True());
    if (isinstance<EmptySet>(subset))
        return boolptr_t(new True());
    return boolptr_t(new Includes(subset, this->copyToSetPtr()));
}

setptr_t Set::add(setptr_t)
{
    return setptr_t(new UniversalSet());
}

setptr_t Set::mul(setptr_t)
{
    return setptr_t(new UniversalSet());
}

setptr_t Set::pow(setptr_t)
{
    return setptr_t(new UniversalSet());
}

objptr_t Set::pow(objptr_t b)
{
    if (isinstance<Integer>(b))
        return this->productpow(*dynamic_cast<Integer *>(b.get()));
    else if (isinstance<Set>(b))
        return this->pow(dynamic_cast<Set *>(b.get())->copyToSetPtr());
    return Object::pow(b);
}