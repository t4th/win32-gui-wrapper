#include "thWidth.h"
#include "thWindow.h"
#include "thForm.h"
#include "thMDIChild.h"

const int thWidth::getValue(void) const
{
    TH_ENTER_FUNCTION;
    RECT         rcClient = { 0 };
    int          returnValue = 0;
    thForm *     pForm = NULL;
    thMDIChild * pMdiChild = NULL;

    pForm = dynamic_cast<thForm*>(&m_pParent);
    pMdiChild = dynamic_cast<thMDIChild*>(&m_pParent);

    // thForm and MDIChild are exceptions due to the fact
    // that their border need to be taken into account
    if (pForm || pMdiChild) {
        //right and bottom members contain the width and height of the window
        GetClientRect(this->m_pParent.GetHandle(), &rcClient);

        returnValue = rcClient.right + rcClient.left;
    }
    else { // child windows should never use GetMenu
        GetWindowRect(this->m_pParent.GetHandle(), &rcClient);
        //AdjustWindowRectEx(&rcClient, 0, FALSE, 0);
        returnValue = rcClient.right - rcClient.left;
    }

    TH_LEAVE_FUNCTION;
    return returnValue;
}

void thWidth::setValue(int newValue)
{
    TH_ENTER_FUNCTION;
    RECT     rcClient = { 0 };
    DWORD    dwStyle = 0;
    DWORD    dwExStyle = 0;
    HMENU    hMenu = 0;
    BOOL     fResult = FALSE;
    thForm * pForm = 0;

    pForm = dynamic_cast<thForm*>(&m_pParent);

    dwStyle = GetWindowLongPtr(this->m_pParent.GetHandle(), GWL_STYLE);

    if (pForm) {
        hMenu = GetMenu(this->m_pParent.GetHandle());
    }

#if 0
    // dwExStyle cause wrong calculations
    dwExStyle = GetWindowLongPtr(this->m_pParent->GetHandle(), GWL_EXSTYLE);
#endif

    rcClient = { 0, 0, newValue, this->m_pParent.Height };

    fResult = AdjustWindowRectEx(&rcClient, dwStyle, hMenu ? TRUE : FALSE, dwExStyle);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("AdjustWindowRectEx failed with error = 0x%X"), GetLastError());
    }

    if (newValue > 0) {
        fResult = FALSE;

        fResult = SetWindowPos(
            this->m_pParent.GetHandle(),
            NULL,
            0,
            0,
            rcClient.right - rcClient.left,
            rcClient.bottom - rcClient.top,
            SWP_NOZORDER | SWP_NOMOVE);

        if (fResult) {
            if (NULL == pForm) {
                this->m_pParent.StoreCurrentRect();
            }
        } else {
            MSG_ERROR(TEXT("SetWindowPos failed with error = 0x%X"), GetLastError());
        }
    }

    TH_LEAVE_FUNCTION;
}

thWidth::thWidth(thWindow & a_pParent) : thParam(a_pParent)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}
thWidth::~thWidth()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thWidth& thWidth::operator=(const int & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<int>::operator=(a_arg);

    TH_LEAVE_FUNCTION;

    return *this;
}
thWidth & thWidth::operator=(const thWidth & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<int>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
int thWidth::operator+(const int & a_arg)
{
    TH_ENTER_FUNCTION;
    int val = thParam<int>::operator+(a_arg);
    TH_LEAVE_FUNCTION;
    return val;
}
thWidth & thWidth::operator+=(const int & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<int>::operator+=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thWidth & thWidth::operator-=(const int & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<int>::operator-=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thWidth::operator int() const
{
    TH_ENTER_FUNCTION;
    int val = thParam<int>::operator int();
    TH_LEAVE_FUNCTION;
    return val;
}