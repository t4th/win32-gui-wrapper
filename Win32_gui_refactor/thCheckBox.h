#pragma once
#include "thWindow.h"

class thCheckBox : public thWindow
{
protected:
private:
    static int              m_indexPool;

                            thCheckBox();
    int                     getDebugIndex();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thCheckBox(thWindow *, int, int);
                            ~thCheckBox();

    thEventCallbackFunc_t   OnClick; // = BN_CLICKED,
    thEventCallbackFunc_t   OnDisable; // = BN_DISABLE,
    thEventCallbackFunc_t   OnDoubleClicked; // = BN_DOUBLECLICKED,
    thEventCallbackFunc_t   OnSetFocus; // = BN_SETFOCUS,
    thEventCallbackFunc_t   OnKillFocus; // = BN_KILLFOCUS
};

