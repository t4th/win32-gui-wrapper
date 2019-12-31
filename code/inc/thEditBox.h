#pragma once
#include "thWindow.h"

class thEditBox : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

                            thEditBox();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         onKeyDown(WPARAM, LPARAM); //WM_KEYDOWN
public:
                            thEditBox(thWindow *, int, int);
                            ~thEditBox();

    void                    SetCaretPosition(uint32_t);

    thEventCallbackFunc_t   OnKeyDown; // WM_KEYDOWN
};
