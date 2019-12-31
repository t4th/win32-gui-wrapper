#pragma once
#include "thParam.h"

class thWidth : public thParam<int>
{
private:
protected:
    const int   getValue(void) const;
    void        setValue(int);
public:
                thWidth() = delete;
                thWidth(thWindow & a_pParent);
                ~thWidth();

    thWidth &   operator= (const int &);
    thWidth &   operator= (const thWidth &);
    int         operator+ (const int &);
    thWidth &   operator+=(const int &);
    thWidth &   operator-=(const int &);
    operator    int() const;
};

