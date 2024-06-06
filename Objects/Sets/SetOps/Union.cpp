#include "Union.h"
#include "False.h"

boolptr_t Union::contains(objptr_t element)
{
    boolptr_t result(new False());
    for (size_t i = 0; i < this->args.size(); i++)
        result = result || (element->operator==(this->args[i]));
    return result;
}