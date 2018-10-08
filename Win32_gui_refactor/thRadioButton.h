#pragma once
#include "thWindow.h"

class thRadioButton : public thWindow
{
protected:
private:
    static int              m_indexPool;

                            thRadioButton();
    int                     getDebugIndex();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thRadioButton(thWindow *, int, int);
                            ~thRadioButton();

    thEventCallbackFunc_t   OnClick; // = BN_CLICKED,
    thEventCallbackFunc_t   OnDisable; // = BN_DISABLE,
    thEventCallbackFunc_t   OnDoubleClicked; // = BN_DOUBLECLICKED,
    thEventCallbackFunc_t   OnSetFocus; // = BN_SETFOCUS,
    thEventCallbackFunc_t   OnKillFocus; // = BN_KILLFOCUS
};

