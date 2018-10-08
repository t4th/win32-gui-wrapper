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
public:
                            thEditBox(thWindow *, int, int);
                            ~thEditBox();
};
