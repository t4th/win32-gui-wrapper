#include "thPosY.h"
#include "thWindow.h"
#include "thForm.h"

thPosY::thPosY(thWindow & a_pParent) : thParam(a_pParent)
{
}

const LONG thPosY::getValue(void) const
{
    TH_ENTER_FUNCTION;
    RECT rcClient = { 0 };

    GetWindowRect( this->m_pParent.GetHandle(), &rcClient);

    const HWND parent = GetParent( this->m_pParent.GetHandle());

    MapWindowPoints( HWND_DESKTOP, parent, ( LPPOINT) &rcClient, 2);

    TH_LEAVE_FUNCTION;
    return rcClient.top;
}
void thPosY::setValue( LONG a_dNewY)
{
    TH_ENTER_FUNCTION;
    const thForm * const pForm = dynamic_cast< thForm*>( &m_pParent);

    //GetWindowRect(m_pParent->getHandle(), &rcClient);

    BOOL result = SetWindowPos(
        this->m_pParent.GetHandle(),
        NULL,
        m_pParent.X, //x
        a_dNewY, //y
        0, //cx - new width
        0, //cy - no change
        SWP_NOZORDER | SWP_NOSIZE
    ); // SWP_NOMOVE ignore x,y | SWP_NOSIZE ignore cx, cy

    if ( result)
    {
        if ( nullptr == pForm)
        {
            this->m_pParent.StoreCurrentRect();
        }
    }
    else
    {
        MSG_ERROR(TEXT("SetWindowPos failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thPosY & thPosY::operator=(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thPosY & thPosY::operator=(const thPosY & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
LONG thPosY::operator+(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    LONG val = thParam<LONG>::operator+(a_arg);
    TH_LEAVE_FUNCTION;
    return val;
}
thPosY & thPosY::operator+=(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator+=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thPosY & thPosY::operator-=(const LONG & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<LONG>::operator-=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thPosY::operator LONG() const
{
    TH_ENTER_FUNCTION;
    LONG val = thParam<LONG>::operator LONG();
    TH_LEAVE_FUNCTION;
    return val;
}