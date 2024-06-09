#include "Tan.h"
#include "PI.h"
#include "Infinity.h"
#include "Derivative.h"
#include "Float.h"
#include "Rational.h"

Tan::Tan(exprptr_t arg) : TrigFunction("tan", arg, setptr_t(new RealSet()), setptr_t(new RealSet()))
{
    this->period = 1;
    exprptr_t pi(new PI());
    exprptr_t _0(new Integer(0));
    exprptr_t _1(new Integer(1));
    exprptr_t _2(new Integer(2));
    exprptr_t _3(new Integer(3));
    exprptr_t _4(new Integer(4));
    exprptr_t _5(new Integer(5));
    exprptr_t _6(new Integer(6));
    exprptr_t _12(new Integer(12));
    exprptr_t sqrt_2(_2->sqrt());
    exprptr_t sqrt_3(_3->sqrt());
    exprptr_t sqrt_6(_6->sqrt());
    exprptr_t inf(new Infinity());
    // tan(0)=0
    this->special.push_back({_0, _0});
    // tan(15°=pi/12)=2-√3
    this->special.push_back({pi / _12, _2 - sqrt_3});
    // tan(30°=pi/6)=√3/3;
    this->special.push_back({pi / _6, sqrt_3 / _3});
    // tan(45°=pi/4)=1
    this->special.push_back({pi / _4, _1});
    // tan(60°=pi/3)=√3
    this->special.push_back({pi / _3, sqrt_3});
    // tan(75°=5pi/12)=2+√3
    this->special.push_back({_5 * pi / _12, _2 + sqrt_3});
    // tan(90°=pi/2)=oo
    this->special.push_back({pi / _2, inf});
}

exprptr_t Tan::_simplify()
{
    TrigFunction *tf = dynamic_cast<TrigFunction *>(this->copyThis());
    exprptr_t pi(new PI());
    exprptr_t coef = tf->arg->getCoef(pi);
    if (can_reduce(coef))
    {
        reducePeriod(tf->arg, coef);
        // tan(x+pi)=tan(x)
        if (isinstance<True>(coef >= Integer(1)))
        {
            tf->arg = tf->arg - pi;
            coef = coef - Integer(1);
        }
        // tan(x-pi)=tan(x)
        if (isinstance<True>(coef <= Integer(-1)))
        {
            tf->arg = tf->arg + pi;
            coef = coef + Integer(1);
        }
        /*exprptr_t half = exprptr_t(new Rational(1, 2));
        exprptr_t _half = exprptr_t(new Rational(-1, 2));
        exprptr_t half_pi = pi * half;
        // tan(x+pi/2)=-cot(x)
        if (isinstance<True>(coef >= half))
        {
            tf->arg = tf->arg - half_pi;
            coef = coef - half;
            exprptr_t ret = exprptr_t(new Cot(tf->arg))>opposite();
            delete tf;
            return ret;
        }
        // tan(x-pi/2)=-cot(x)
        if (isinstance<True>(coef <= _half))
        {
            tf->arg = tf->arg + half_pi;
            coef = coef + half;
            exprptr_t ret = exprptr_t(new Cot(tf->arg))->opposite();
            delete tf;
            return ret;
        }*/
    }
    // tan(-x)=-tan(x)
    if (isinstance<Mul>(tf->arg) && !isinstance<True>(tf->arg->getCoef(exprptr_t(new Integer(-1))) == Integer(0)))
    {
        tf->arg = tf->arg->opposite();
    }
    exprptr_t ret = tf->matchSpecial();
    delete tf;
    return ret;
}

exprptr_t Tan::diff(exprptr_t target)
{
    exprptr_t result = Expression::diff(target);
    if (!isinstance<Derivative>(result) && isinstance<Symbol>(target))
        result = (exprptr_t(new Tan(this->arg))->pow(Integer(2)) + Integer(1)) * this->arg->diff(target);
    return result;
}

setptr_t tan(Interval *b)
{
    if (!isinstance<Number>(b->start) || !isinstance<Number>(b->end))
        return nullptr;
    double start = dynamic_cast<Number *>(b->start.get())->toDouble();
    double end = dynamic_cast<Number *>(b->end.get())->toDouble();
    // 没有考虑单调性
    return setptr_t(new Interval(exprptr_t(new Float(std::tan(start))),
                                 exprptr_t(new Float(std::tan(end))), b->left_open, b->right_open));
}

setptr_t tan(EnumSet *b)
{
    elements_t elements;
    for (size_t i = 0; i < b->elements.size(); i++)
    {
        if (!isinstance<Number>(b->elements[i]))
            return nullptr;
        double n = dynamic_cast<Number *>(b->elements[i].get())->toDouble();
        elements.push_back(exprptr_t(new Float(std::tan(n))));
    }
    return setptr_t(new EnumSet(elements));
}

setptr_t tan(setptr_t b)
{
    setptr_t result = nullptr;
    if (isinstance<Interval>(b))
        result = tan(dynamic_cast<Interval *>(b.get()));
    else if (isinstance<EnumSet>(b))
        result = tan(dynamic_cast<EnumSet *>(b.get()));
    if (result == nullptr)
        return setptr_t(new Interval(exprptr_t(new Integer(-1)), exprptr_t(new Integer(1)), false, false));
    return result;
}

setptr_t Tan::belongto()
{
    setptr_t arg_belong = this->arg->belongto();
    return tan(arg_belong);
}