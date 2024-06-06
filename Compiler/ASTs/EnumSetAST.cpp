#include "EnumSetAST.h"
#include "EnumSet.h"

objptr_t EnumSetAST::exec(Runtime *runtime)
{
    elements_t elements;
    for (size_t i = 0; i < this->children.size(); i++)
    {
        objptr_t t = this->children[i]->exec(runtime);
        if (t != nullptr)
            elements.push_back(t);
    }
    if (elements.size() == 0)
        return setptr_t(new EmptySet());
    return setptr_t(new EnumSet(elements));
}