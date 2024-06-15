#include <stdexcept>

#include "Mapping.h"
#include "Tuple.h"
#include "True.h"

objptr_t Mapping::operator()(funcargs_t args)
{
    if (isinstance<False>(in_domain(domain, args)))
        throw std::runtime_error("[" + this->name + "超出定义域");
    if (this->sections.size() != 0)
    {
        bool flag = true; // 是否有不在定义域内的参数
        for (size_t i = 0; i < this->sections.size(); i++)
        {
            boolptr_t contain = in_domain(this->sections[i].domain, args);
            if (isinstance<True>(contain))
                return this->sections[i](args);
            if (!isinstance<False>(contain))
                flag = false;
        }
        if (flag)
            throw std::runtime_error("[" + this->name + "]超出定义域");
    }
    return objptr_t(new Function(shared_from_this(), args));
}