#pragma once
#include "thParam.h"

class thText : public thParam<thString>
{
private:
protected:
    const thString      getValue(void) const;
    void                setValue(thString newValue);
public:
                        thText() = delete;
                        thText(thWindow & a_pParent);
                        ~thText();

    thText &             operator =  (const thString &);
    thText &             operator =  (const thText   &);
    thString             operator +  (const thString &);
    thText &             operator += (const thString &);
                         operator thString () const;
};

