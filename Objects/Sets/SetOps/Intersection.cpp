#include "Intersection.h"
#include "True.h"

boolptr_t Intersection::contains(objptr_t element)
{
    boolptr_t result(new True());
    for (size_t i = 0; i < this->args.size(); i++)
        result = result && (element->operator==(this->args[i]));
    return result;
}