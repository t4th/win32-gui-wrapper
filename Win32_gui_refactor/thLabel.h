#pragma once
#include "thWindow.h"

class thLabel : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

                            thLabel();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thLabel(thWindow *, int, int);
                            ~thLabel();

    thEventCallbackFunc_t   OnClick;          // = STN_CLICKED,
    thEventCallbackFunc_t   OnDoubleClicked;  // = STN_DBLCLK,
    thEventCallbackFunc_t   OnDisable;        // = STN_DISABLE,
    thEventCallbackFunc_t   OnEnable;         // = STN_ENABLE,
};

