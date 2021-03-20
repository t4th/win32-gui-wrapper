#pragma once
#include "thButton.h"

class thGroupBox : public thWindow
{
protected:
private:
    static int              m_indexPool;

                            thGroupBox();
    int                     getDebugIndex();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thGroupBox(thWindow *, int, int);
                            ~thGroupBox();

#if 0
    thEventCallbackFunc_t   OnClick{nullptr}; // = BN_CLICKED,
    //thEventCallbackFunc_t OnPaint{nullptr}; // = BN_PAINT,
    //thEventCallbackFunc_t OnHilite{nullptr}; // = BN_HILITE,
    //thEventCallbackFunc_t OnUnhilite{nullptr}; // = BN_UNHILITE,
    thEventCallbackFunc_t   OnDisable{nullptr}; // = BN_DISABLE,
    //thEventCallbackFunc_t OnPushed{nullptr}; // = BN_HILITE,
    //thEventCallbackFunc_t OnUnpushed{nullptr}; // = BN_UNHILITE,
    thEventCallbackFunc_t   OnDoubleClicked{nullptr}; // = BN_DOUBLECLICKED,
    thEventCallbackFunc_t   OnSetFocus{nullptr}; // = BN_SETFOCUS,
    thEventCallbackFunc_t   OnKillFocus{nullptr}; // = BN_KILLFOCUS
#endif
};

