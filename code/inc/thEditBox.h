#pragma once
#include "thWindow.h"

class thEditBox : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    LRESULT                 processCommandMessage( HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam);
    LRESULT                 processNotifyMessage( HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam);
    virtual LRESULT         onKeyDown(  WPARAM a_wParam, LPARAM a_lParam);
public:
                            thEditBox() = delete;
                            thEditBox( thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thEditBox() = default;

    void                    SetCaretPosition( uint32_t a_u32Position);

    thEventCallbackFunc_t   OnKeyDown{ nullptr};
};
