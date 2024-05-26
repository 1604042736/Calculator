#include "Number.h"
#include "Integer.h"

exprptr_t Number::diff(exprptr_t)
{
    // 对常量求导为0
    return exprptr_t(new Integer(0));
}