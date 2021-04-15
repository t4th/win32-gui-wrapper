#pragma once
#include "thParam.h"

class thPosX : public thParam<LONG>
{
protected:
private:
    const LONG  getValue(void) const;
    void        setValue(LONG);
public:
                thPosX() = delete;
                thPosX(thWindow & a_pParent);
                ~thPosX() = default;

    thPosX &     operator =(const LONG &);
    thPosX &     operator =(const thPosX &);
    LONG         operator +(const LONG &);
    thPosX &     operator+=(const LONG &);
    thPosX &     operator-=(const LONG &);
    operator     LONG() const;
};

