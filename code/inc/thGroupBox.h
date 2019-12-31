#pragma once
#include "thButton.h"

class thGroupBox : public thWindow
{
protected:
private:
    static int          m_indexPool;

                        thGroupBox();
    int                 getDebugIndex();
    LRESULT             processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT             processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                        thGroupBox(thWindow *, int, int);
                        ~thGroupBox();

#if 0
                        struct
                        {
                            thEventCallbackFunc_t OnClick; // = BN_CLICKED,
                            //thEventCallbackFunc_t OnPaint; // = BN_PAINT,
                            //thEventCallbackFunc_t OnHilite; // = BN_HILITE,
                            //thEventCallbackFunc_t OnUnhilite; // = BN_UNHILITE,
                            thEventCallbackFunc_t OnDisable; // = BN_DISABLE,
                            //thEventCallbackFunc_t OnPushed; // = BN_HILITE,
                            //thEventCallbackFunc_t OnUnpushed; // = BN_UNHILITE,
                            thEventCallbackFunc_t OnDoubleClicked; // = BN_DOUBLECLICKED,
                            thEventCallbackFunc_t OnSetFocus; // = BN_SETFOCUS,
                            thEventCallbackFunc_t OnKillFocus; // = BN_KILLFOCUS
                        } Events;
#endif
};

