#pragma once
#include "thWindow.h"

class thRadioButton : public thWindow
{
protected:
private:
    static int              m_indexPool;

    int                     getDebugIndex();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thRadioButton() = delete;
                            thRadioButton(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thRadioButton();

    thEventCallbackFunc_t   OnClick{nullptr};
    thEventCallbackFunc_t   OnDisable{nullptr};
    thEventCallbackFunc_t   OnDoubleClicked{nullptr};
    thEventCallbackFunc_t   OnSetFocus{nullptr};
    thEventCallbackFunc_t   OnKillFocus{nullptr};
};

