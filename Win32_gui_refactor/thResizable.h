#pragma once
#include "thParam.h"

// Parameter of thForm Window. Can be used to check if Window is resizable and to turn on/off resizablity.
// if (thForm->Resizable) {}
// thForm->Resizable = true; thForm->Resizable = false;
class thResizable : public thParam<bool_t>
{
protected:
private:
    const bool_t    getValue(void) const;
    void            setValue(bool_t);
public:
                    thResizable();
                    ~thResizable();

    thResizable &   operator =(const bool_t &);
    operator        bool_t() const;
};

