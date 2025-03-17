#pragma once

#include "thTypes.h"
#include "thConfig.h"

#include "thWindow.h"

/*
Generalization of win32 styles.
In this class, each style provided as TMask is of type bool, which can be read, set or clear.
Interacting is supposed to be programmer-friendly thanks to provided operator overloadings.

- TStyleType should be: GWL_EXSTYLE or GWL_STYLE
- TMask is style mask to be used
*/
template <int TStyleType, int TMask>
class thSetGetStyle : public thParam<bool_t>
{
protected:
private:
    const bool_t    getValue(void) const{
        TH_ENTER_FUNCTION;
        bool_t      fResult = false;
        LONG_PTR    lpWindowStyle = 0;

        // GetWindowLongPtr return 0 on error
        lpWindowStyle = GetWindowLongPtr(this->m_pParent.GetHandle(), TStyleType);

        if (lpWindowStyle) {
            if (TMask == (lpWindowStyle & TMask)) {
                fResult = true;
            }
        }
        else {
            MSG_WARNING(TEXT("GetWindowLongPtr failed with error: 0x%X"), GetLastError());
        }

        TH_LEAVE_FUNCTION;
        return fResult;
    }
    void            setValue(bool_t a_fResizeable)
    {
        TH_ENTER_FUNCTION;
        BOOL        fResult = FALSE;
        LONG_PTR    lpWindowStyle = 0;
        LONG_PTR    lpResult = 0;

        // GetWindowLongPtr return 0 on error
        lpWindowStyle = GetWindowLongPtr(this->m_pParent.GetHandle(), TStyleType);

        if (lpWindowStyle) {
            if (a_fResizeable) {
                lpWindowStyle |= TMask;
            }
            else {
                lpWindowStyle &= ~TMask;
            }

            // SetWindowLongPtr return 0 on error
            lpResult = SetWindowLongPtr(this->m_pParent.GetHandle(), TStyleType, lpWindowStyle);

            if (0 == lpResult) {
                MSG_WARNING(TEXT("SetWindowLongPtr failed with error: 0x%X"), GetLastError());
            }
        }
        else {
            MSG_WARNING(TEXT("GetWindowLongPtr failed with error: 0x%X"), GetLastError());
        }

        TH_LEAVE_FUNCTION;
    }
public:
    thSetGetStyle() = delete;

    thSetGetStyle(thWindow & a_pParent) : thParam(a_pParent)
    {
    }

    ~thSetGetStyle() = default;

    thSetGetStyle & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thParam<bool_t>::operator=(a_arg);

        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        TH_ENTER_FUNCTION;
        bool_t val = thParam<bool_t>::operator bool_t();
        TH_LEAVE_FUNCTION;
        return val;
    }
};
