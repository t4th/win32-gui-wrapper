#pragma once
#include "thWindow.h"
#include "thWindowStyles.h"

class thButton : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thButton() = delete;
                            thButton(thWindow *, int, int);
                            ~thButton();

    thBS_DefaultPush        DefaultPushStyle; // Change Button look

    thEventCallbackFunc_t   OnClick{nullptr};
    thEventCallbackFunc_t   OnPaint{nullptr};
    thEventCallbackFunc_t   OnHilite{nullptr};
    thEventCallbackFunc_t   OnUnhilite{nullptr};
    thEventCallbackFunc_t   OnDisable{nullptr};
    thEventCallbackFunc_t   OnPushed{nullptr};
    thEventCallbackFunc_t   OnUnpushed{nullptr};
    thEventCallbackFunc_t   OnDoubleClicked{nullptr};
    thEventCallbackFunc_t   OnSetFocus{nullptr};
    thEventCallbackFunc_t   OnKillFocus{nullptr};

};

