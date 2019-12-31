#pragma once
#include "thWindow.h"

class thRichEdit : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

                            thRichEdit();
    void                    SetAStyle(int, COLORREF, COLORREF, int, const char *);
    LRESULT                 onSetText(LPARAM);
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thRichEdit(thWindow *, int, int);
                            ~thRichEdit();
};
