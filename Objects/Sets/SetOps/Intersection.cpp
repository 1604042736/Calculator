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