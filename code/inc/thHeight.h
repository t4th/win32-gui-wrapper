#pragma once
#include "thParam.h"

class thHeight : public thParam<int>
{
private:
protected:
    const int   getValue(void) const;
    void        setValue(int);
public:
                thHeight() = delete;
                thHeight(thWindow & a_pParent);
                ~thHeight();
    
    thHeight &  operator= (const int &);
    thHeight &  operator= (const thHeight &);
    int         operator+ (const int &);
    thHeight &  operator+=(const int &);
    thHeight &  operator-=(const int &);
    operator    int() const;
};

