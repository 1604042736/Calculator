#pragma once

#include <vector>
#include <map>
#include <bitset>

#include "Object.h"
#include "Expression.h"
#include "Mapping.h"
#include "Integer.h"
#include "EmptySet.h"

#define EXPR_FLAG 0
#define BOOL_FLAG 1

typedef std::bitset<16> flags_t;
typedef std::map<std::string, objptr_t> scope_t;

/*运行时*/
class Runtime
{
public:
    Runtime();

    objptr_t findName(std::string);
    void defName(std::string, objptr_t);

    void enterScope();
    void leaveScope();
    scope_t &curScope();

    std::vector<scope_t> scopes; // 作用域, 上层作用域放在大索引处
    flags_t flags;               // 标志位
};