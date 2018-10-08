#include "thResizable.h"
#include "thWindow.h"
#include "thForm.h"

thResizable::thResizable()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thResizable::~thResizable()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

const bool_t thResizable::getValue(void) const
{
    TH_ENTER_FUNCTION;
    bool_t      fResult = false;
    LONG_PTR    lpWindowStyle = 0;

    // GetWindowLongPtr return 0 on error
    lpWindowStyle = GetWindowLongPtr(this->m_pParent->GetHandle(), GWL_STYLE);

    if (lpWindowStyle) {
        if (WS_THICKFRAME == (lpWindowStyle & WS_THICKFRAME)) {
            fResult = true;
        }
    }
    else {
        MSG_WARNING(L"GetWindowLongPtr failed with error: 0x%X", GetLastError());
    }

    TH_LEAVE_FUNCTION;
    return fResult;
}

void thResizable::setValue(bool_t a_fResizeable)
{
    TH_ENTER_FUNCTION;
    BOOL        fResult = FALSE;
    LONG_PTR    lpWindowStyle = 0;
    LONG_PTR    lpResult = 0;

    // GetWindowLongPtr return 0 on error
    lpWindowStyle = GetWindowLongPtr(this->m_pParent->GetHandle(), GWL_STYLE);

    if (lpWindowStyle) {
        if (a_fResizeable) {
            lpWindowStyle |= WS_THICKFRAME;
        }
        else {
            lpWindowStyle &= ~WS_THICKFRAME;
        }

        // SetWindowLongPtr return 0 on error
        lpResult = SetWindowLongPtr(this->m_pParent->GetHandle(), GWL_STYLE, lpWindowStyle);

        if (0 == lpResult) {
            MSG_WARNING(L"SetWindowLongPtr failed with error: 0x%X", GetLastError());
        }
    }
    else {
        MSG_WARNING(L"GetWindowLongPtr failed with error: 0x%X", GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thResizable & thResizable::operator=(const bool_t & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<bool_t>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}

thResizable::operator bool_t() const
{
    TH_ENTER_FUNCTION;
    bool_t val = thParam<bool_t>::operator bool_t();
    TH_LEAVE_FUNCTION;
    return val;
}