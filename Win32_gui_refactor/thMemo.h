#pragma once
#include "thWindow.h"

class thMemo : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

                            thMemo();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thMemo(thWindow *, int, int);
                            ~thMemo();

    void                    ScrollDown();
};
