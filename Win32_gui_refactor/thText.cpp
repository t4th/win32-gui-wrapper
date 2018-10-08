#include "thText.h"
#include "thWindow.h"

const thString thText::getValue(void) const
{
    TH_ENTER_FUNCTION;
    thString    returnValue;
    TCHAR *     pBuffer = NULL;
    int         nLen = 0;

    nLen = GetWindowTextLength(this->m_pParent->GetHandle());

    if (nLen > 0) {
        pBuffer = new TCHAR[nLen + 1]; //+1 for '\0'

        if (pBuffer) {
            BOOL fResult = FALSE;

            fResult = GetWindowText(this->m_pParent->GetHandle(), pBuffer, nLen + 1);

            if (fResult) {
                returnValue = thString(pBuffer, nLen);
            }

            delete [] pBuffer;
        }
    }

    TH_LEAVE_FUNCTION;
    return returnValue;
}
void thText::setValue(thString newValue)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    DWORD   dwLastError = 0;

    SetLastError(dwLastError);

    tResult = SendMessage(
        this->m_pParent->GetHandle(),
        WM_SETTEXT,
        NULL,
        (LPARAM)(newValue.c_str()));

    dwLastError = GetLastError();

    if (TRUE != tResult) {
        if (dwLastError) {
            MSG_ERROR(L"SendMessage WM_SETTEXT failed with error = 0x%X, tResult = %d", dwLastError, tResult);
        }
    }

    TH_LEAVE_FUNCTION;
}
thText::thText()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}
thText::~thText()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thText & thText::operator=(const thString & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<thString>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thText & thText::operator=(const thText & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<thString>::operator=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thString thText::operator+(const thString & a_arg)
{
    TH_ENTER_FUNCTION;
    thString val = thParam<thString>::operator+(a_arg);
    TH_LEAVE_FUNCTION;
    return val;
}
thText & thText::operator+=(const thString & a_arg)
{
    TH_ENTER_FUNCTION;
    thParam<thString>::operator+=(a_arg);

    TH_LEAVE_FUNCTION;
    return *this;
}
thText::operator thString () const
{
    TH_ENTER_FUNCTION;
    thString val = thParam<thString>::operator thString();
    TH_LEAVE_FUNCTION;
    return val;
}
