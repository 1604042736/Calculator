#include "E.h"
#include "Integer.h"
#include "Rational.h"

exprptr_t E::eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    Rational s(0), a(1);
    Integer keep_2 = keep * 2;
    for (Integer i = 1; i < keep_2; i = i + 1)
    {
        s = s + (Integer)1 / a;
        a = a * i;
    }
    return s.eval(keep);
}