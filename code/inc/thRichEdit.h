#pragma once
#include "thWindow.h"

class thRichEdit : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    void                    registerClass();

    void                    SetAStyle(int, COLORREF, COLORREF, int, const char *);
    LRESULT                 onSetText(LPARAM);
    LRESULT                 onContextMenu(WPARAM a_wParam, LPARAM a_lParam);
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:

                            thRichEdit() = delete;
                            thRichEdit(thWindow *, int, int);
                            ~thRichEdit();
};
