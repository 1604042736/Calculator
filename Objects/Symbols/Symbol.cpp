#include <algorithm>

#include "Symbol.h"

std::vector<std::string> names = {}; // 已经创建过的名字

Symbol::Symbol(std::string name) : name(name)
{
    if (std::find(names.begin(), names.end(), name) == names.end())
        names.push_back(name);
}