#pragma once

#include "Object.h"

class Boolean;
typedef std::shared_ptr<Boolean> boolptr_t;

/*布尔表达式*/
class Boolean : virtual public Object
{
public:
    virtual boolptr_t operator&&(boolptr_t);
    virtual boolptr_t operator&&(Boolean &);
    virtual boolptr_t operator&&(Boolean &&);
    friend boolptr_t operator&&(boolptr_t, boolptr_t);
    friend boolptr_t operator&&(boolptr_t, Boolean &);
    friend boolptr_t operator&&(boolptr_t, Boolean &&);
    virtual boolptr_t operator||(boolptr_t);
    virtual boolptr_t operator||(Boolean &);
    virtual boolptr_t operator||(Boolean &&);
    friend boolptr_t operator||(boolptr_t, boolptr_t);
    friend boolptr_t operator||(boolptr_t, Boolean &);
    friend boolptr_t operator||(boolptr_t, Boolean &&);
    virtual boolptr_t operator!();
    friend boolptr_t operator!(boolptr_t);

    virtual bool operator==(boolptr_t);
    virtual bool operator==(Boolean &);
    virtual bool operator==(Boolean &&);
    friend bool operator==(boolptr_t, boolptr_t);
    friend bool operator==(boolptr_t, Boolean &);
    friend bool operator==(boolptr_t, Boolean &&);
    virtual bool operator!=(boolptr_t);
    virtual bool operator!=(Boolean &);
    virtual bool operator!=(Boolean &&);
    friend bool operator!=(boolptr_t, boolptr_t);
    friend bool operator!=(boolptr_t, Boolean &);
    friend bool operator!=(boolptr_t, Boolean &&);

    virtual boolptr_t operator&&(objptr_t);
    virtual boolptr_t operator||(objptr_t);

    virtual boolptr_t inline copyToBoolPtr() { return boolptr_t(dynamic_cast<Boolean *>(this->copyThis())); }

    /*化简*/
    virtual boolptr_t simplify();
    /*化简一次*/
    virtual boolptr_t _simplify()
    {
        return this->copyToBoolPtr();
    }
};

boolptr_t to_boolean(bool);