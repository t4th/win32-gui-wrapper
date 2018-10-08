#include "thPosX.h"
#include "thWindow.h"
#include "thForm.h"

thPosX::thPosX()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}


thPosX::~thPosX()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

const LONG thPosX::getValue(void) const
{
    TH_ENTER_FUNCTION;
    RECT rcClient = { 0 };

    GetWindowRect(this->m_pParent->GetHandle(), &rcClient);
    MapWindowPoints(HWND_DESKTOP, GetParent(this->m_pParent->GetHandle()), (LPPOINT)&rcClient, 2);

    TH_LEAVE_FUNCTION;
    return rcClient.left;
}
void thPosX::setValue(LONG a_dNewX)
{
    TH_ENTER_FUNCTION;
    BOOL        fResult = FALSE;
    thForm *    pForm = NULL;

    pForm = dynamic_cast<thForm*>(m_pParent);

    fResult = SetWindowPos(this->m_pParent->GetHandle(), NULL,
        a_dNewX,        // x
        m_pParent->Y,   // y
        0,              // cx - ignored with SWP_NOSIZE
        0,              // cy - ignored with SWP_NOSIZE
        SWP_NOZORDER | SWP_NOSIZE); // SWP_NOMOVE ignore x,y | SWP_NOSIZE ignore cx, cy

    if (fResult) {
        if (NULL == pForm) {
            this->m_pParent->StoreCurrentRect(); // store new RECT
        }
    }
    else {
        MSG_ERROR(L"SetWindowPos failed with error = 0x%X", GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thPosX & thPosX::operator=(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thPosX & thPosX::operator=(const thPosX & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
LONG thPosX::operator+(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    LONG val = thParam<LONG>::operator+(a_arg);
    TH_LEAVE_FUNCTION;
    return val;
}
thPosX & thPosX::operator+=(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator+=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thPosX & thPosX::operator-=(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator-=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thPosX::operator LONG() const
{
    TH_ENTER_FUNCTION;
    LONG val = thParam<LONG>::operator LONG();
    TH_LEAVE_FUNCTION;
    return val;
}