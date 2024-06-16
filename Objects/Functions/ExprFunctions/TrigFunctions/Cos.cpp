#include "Cos.h"
#include "PI.h"
#include "Sin.h"
#include "Derivative.h"
#include "Float.h"
#include "Rational.h"

Cos::Cos(exprptr_t arg) : TrigFunction("cos", arg)
{
    this->period = 2;
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
    // cos(0)=1
    this->special.push_back({_0, _1});
    // cos(15°=pi/12)=(√6+√2)/4
    this->special.push_back({pi / _12, (sqrt_6 + sqrt_2) / _4});
    // cos(30°=pi/6)=√/2;
    this->special.push_back({pi / _6, sqrt_3 / _2});
    // cos(45°=pi/4)=√2/2
    this->special.push_back({pi / _4, sqrt_2 / _2});
    // cos(60°=pi/3)=1/2
    this->special.push_back({pi / _3, _1 / _2});
    // cos(75°=5pi/12)=(√6-√2)/4
    this->special.push_back({_5 * pi / _12, (sqrt_6 - sqrt_2) / _4});
    // cos(90°=pi/2)=0
    this->special.push_back({pi / _2, _0});

    this->mapping = mappingptr_t(new CosMapping());
}

exprptr_t Cos::_simplify()
{
    TrigFunction *tf = dynamic_cast<TrigFunction *>(this->copyThis());
    exprptr_t pi(new PI());
    exprptr_t coef = tf->arg->getCoef(pi);
    if (can_reduce(coef))
    {
        reducePeriod(tf->arg, coef);
        // cos(x+pi)=-cos(x)
        if (isinstance<True>(coef >= Integer(1)))
        {
            tf->arg = tf->arg - pi;
            coef = coef - Integer(1);
            exprptr_t ret = tf->opposite();
            delete tf;
            return ret;
        }
        // cos(x-pi)=-cos(x)
        if (isinstance<True>(coef <= Integer(-1)))
        {
            tf->arg = tf->arg + pi;
            coef = coef + Integer(1);
            exprptr_t ret = tf->opposite();
            delete tf;
            return ret;
        }
        exprptr_t half = exprptr_t(new Rational(1, 2));
        exprptr_t _half = exprptr_t(new Rational(-1, 2));
        exprptr_t half_pi = pi * half;
        // cos(x+pi/2)=-sin(x)
        if (isinstance<True>(coef >= half))
        {
            tf->arg = tf->arg - half_pi;
            coef = coef - half;
            exprptr_t ret = exprptr_t(new Sin(tf->arg))->opposite();
            delete tf;
            return ret;
        }
        // cos(x-pi/2)=sin(x)
        if (isinstance<True>(coef <= _half))
        {
            tf->arg = tf->arg + half_pi;
            coef = coef + half;
            exprptr_t ret = exprptr_t(new Sin(tf->arg));
            delete tf;
            return ret;
        }
    }
    // cos(-x)=cos(x)
    if (isinstance<Mul>(tf->arg) && !isinstance<True>(tf->arg->getCoef(exprptr_t(new Integer(-1))) == Integer(0)))
    {
        tf->arg = tf->arg->opposite();
    }
    exprptr_t ret = tf->matchSpecial();
    delete tf;
    return ret;
}

exprptr_t Cos::diff(exprptr_t target)
{
    if (!isinstance<Symbol>(target))
        return ExprFunction::diff(target);
    return exprptr_t(new Sin(this->arg))->opposite() * this->arg->diff(target);
}

setptr_t cos(Interval *b)
{
    if (!isinstance<Number>(b->start) || !isinstance<Number>(b->end))
        return nullptr;
    double start = dynamic_cast<Number *>(b->start.get())->toDouble();
    double end = dynamic_cast<Number *>(b->end.get())->toDouble();
    if (end < start)
        std::swap(end, start);
    double min = std::cos(start);
    double max = std::cos(end);
    if (floor((start / M_PI - 0) / 2) < floor((end / M_PI - 0) / 2))
        max = 1;
    if (floor((start / M_PI + 1) / 2) < floor((end / M_PI + 1) / 2))
        min = -1;
    if (max < min)
        std::swap(max, min);
    return setptr_t(new Interval(exprptr_t(new Float(min)),
                                 exprptr_t(new Float(max)),
                                 b->left_open,
                                 b->right_open));
}

setptr_t cos(EnumSet *b)
{
    elements_t elements;
    for (size_t i = 0; i < b->elements.size(); i++)
    {
        if (!isinstance<Number>(b->elements[i]))
            return nullptr;
        double n = dynamic_cast<Number *>(b->elements[i].get())->toDouble();
        elements.push_back(exprptr_t(new Float(std::cos(n))));
    }
    return setptr_t(new EnumSet(elements));
}

setptr_t cos(setptr_t b)
{
    setptr_t result = nullptr;
    if (isinstance<Interval>(b))
        result = cos(dynamic_cast<Interval *>(b.get()));
    else if (isinstance<EnumSet>(b))
        result = cos(dynamic_cast<EnumSet *>(b.get()));
    if (result == nullptr)
        return setptr_t(new Interval(exprptr_t(new Integer(-1)), exprptr_t(new Integer(1)), false, false));
    return result;
}

setptr_t Cos::belongto()
{
    setptr_t arg_belong = this->arg->belongto();
    return cos(arg_belong);
}