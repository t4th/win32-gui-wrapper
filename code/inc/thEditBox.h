#pragma once
#include "thWindow.h"

class thEditBox : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         onKeyDown(WPARAM, LPARAM); //WM_KEYDOWN
public:
                            thEditBox() = delete;
                            thEditBox(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thEditBox();

    void                    SetCaretPosition(uint32_t);

    thEventCallbackFunc_t   OnKeyDown{nullptr};
};
