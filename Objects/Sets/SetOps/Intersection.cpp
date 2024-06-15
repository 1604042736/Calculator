#include "Intersection.h"
#include "True.h"

boolptr_t Intersection::contains(objptr_t element)
{
    boolptr_t result(new True());
    for (size_t i = 0; i < this->args.size(); i++)
        result = result && (element->operator==(this->args[i]));
    return result;
}

setptr_t Intersection::_simplify()
{
    setptr_t result = this->args[0]->simplify();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result->operator&(this->args[i]);
    return result;
}

objptr_t Intersection::replace(objptr_t old, objptr_t _new)
{
    setptr_t result = dynamic_cast<Set *>(this->args[0]->replace(old, _new).get())->copyToSetPtr();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result->operator&(dynamic_cast<Set *>(this->args[i]->replace(old, _new).get())->copyToSetPtr());
    return result;
}