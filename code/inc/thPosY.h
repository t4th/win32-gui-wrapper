#pragma once
#include "thParam.h"

class TWindow;

class thPosY : public thParam<LONG>
{
protected:
private:
    const LONG  getValue(void) const;
    void        setValue(LONG);
public:
                thPosY() = delete;
                thPosY(thWindow & a_pParent);
                ~thPosY() = default;

    thPosY &     operator =(const LONG &);
    thPosY &     operator =(const thPosY &);
    LONG         operator +(const LONG &);
    thPosY &     operator+=(const LONG &);
    thPosY &     operator-=(const LONG &);
    operator     LONG() const;
};